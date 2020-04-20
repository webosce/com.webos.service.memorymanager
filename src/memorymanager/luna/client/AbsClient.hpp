// Copyright (c) 2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0


#ifndef LUNA_CLIENT_ABSCLIENT_HPP_
#define LUNA_CLIENT_ABSCLIENT_HPP_

#include <iostream>

#include <luna-service2/lunaservice.hpp>
#include <pbnjson.hpp>

#include "luna/LunaManager.h"
#include "util/Logger.h"
#include "util/JValueUtil.h"

using namespace std;
using namespace pbnjson;
using namespace LS;

class AbsClient {
public:
    static bool onStatusChange(LSHandle *sh, LSMessage *reply, void *ctx)
    {
        LS::Message response(reply);
        JValue subscriptionPayload = JDomParser::fromString(response.getPayload());
        AbsClient* client = (AbsClient*)ctx;

        bool connected = true;
        if (JValueUtil::getValue(subscriptionPayload, "connected", connected)) {
            client->m_isConnected = connected;
            client->onStatusChange(connected);
        }
        return true;
    }

    AbsClient(string serviceName, const string& sessionId = "")
        : m_serviceName(serviceName),
          m_sessionId(sessionId),
          m_isConnected(false),
          m_handle(nullptr),
          m_timeout(5000)
    {

    }

    virtual ~AbsClient()
    {
        m_serverStatus.cancel();
    }

    virtual void initialize(Handle* handle)
    {
        m_serverStatus.cancel();

        JValue requestPayload = pbnjson::Object();
        requestPayload.put("serviceName", m_serviceName);
        if (!m_sessionId.empty())
            requestPayload.put("sessionId", m_sessionId);
        m_serverStatus = handle->callMultiReply(
                "luna://com.webos.service.bus/signal/registerServerStatus",
                requestPayload.stringify().c_str(),
                onStatusChange,
                this
        );
        m_handle = handle;
    }

    // This callback is called when target service status is changed
    virtual bool onStatusChange(bool isConnected) = 0;

    bool callSync(string key, JValue& callPayload, JValue& returnPayload)
    {
        string url = "luna://" + m_serviceName + "/" + key;

        LunaManager::getInstance().logCall(url, callPayload);
        auto call = m_handle->callOneReply(
            url.c_str(),
            callPayload.stringify().c_str()
        );
        auto reply = call.get(m_timeout);
        if (!reply) {
            Logger::error("No reply during timeout");
            return false;
        }
        if (reply.isHubError()) {
            Logger::error(reply.getPayload());
            return false;
        }
        returnPayload = JDomParser::fromString(reply.getPayload());
        LunaManager::getInstance().logReturn(reply, returnPayload);
        return true;
    }

    bool subscribe(Call& call, string key, JValue& requestPayload, LSFilterFunc callback)
    {
        string url = "luna://" + m_serviceName + "/" + key;

        call.cancel();
        try {
            LunaManager::getInstance().logCall(url, requestPayload);
            call = m_handle->callMultiReply(
                url.c_str(),
                requestPayload.stringify().c_str()
            );
            call.continueWith(callback, this);
        }
        catch (const LS::Error &e) {
            Logger::error(string(e.what()), m_serviceName);
            return false;
        }
        return true;
    }

    string& getServiceName()
    {
        return m_serviceName;
    }

protected:
    string m_serviceName;
    string m_sessionId;
    bool m_isConnected;
    Handle *m_handle;
    int m_timeout;

private:
    Call m_serverStatus;

};

#endif /* LUNA_CLIENT_ABSCLIENT_HPP_ */
