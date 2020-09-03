// Copyright (c) 2018-2020 LG Electronics, Inc.
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

#ifndef LUNA_CLIENT_SAM_H_
#define LUNA_CLIENT_SAM_H_

#include <iostream>
#include <vector>

#include <luna-service2/lunaservice.hpp>
#include <pbnjson.hpp>

#include "interface/IPrintable.h"
#include "interface/IListener.h"
#include "base/RunningList.h"

using namespace std;
using namespace pbnjson;
using namespace LS;

class SAM {
public:
    static void subscribe(const string& sessionId = "");
    static void unsubscribe(const string& sessionId = "");

    static bool close(bool includeForeground, string& errorText);

    static string getForegroundAppId();
    static int getRunningAppCount();

    static void toJson(JValue& json);

    virtual ~SAM(){};

private:
    static bool onStatusChange(LSHandle *sh, LSMessage *reply, void *ctx);
    static bool onGetAppLifeEvents(LSHandle *sh, LSMessage *reply, void *ctx);
    static bool onRunning(LSHandle *sh, LSMessage *reply, void *ctx);

    static string getSessionId(LSMessage *reply)
    {
#if defined(WEBOS_TARGET_DISTRO_WEBOS_AUTO)
        if (LSMessageGetSessionId(reply) != NULL)
            return LSMessageGetSessionId(reply);
#endif
        return "";
    }

    SAM();

    static const int CONTEXT_NOT_EXIST = 0;
    static const int CONTEXT_EXIST = 1;

    static RunningList s_runningList;

};

#endif /* LUNA_CLIENT_SAM_H_ */
