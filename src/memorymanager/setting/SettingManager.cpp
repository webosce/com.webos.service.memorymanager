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

#include "SettingManager.h"

SettingManager::SettingManager()
    : m_lowEnter(DEFAULT_LOW_ENTER)
    , m_lowExit(DEFAULT_LOW_EXIT)
    , m_criticalEnter(DEFAULT_CRITICAL_ENTER)
    , m_criticalExit(DEFAULT_CRITICAL_EXIT)
{
}

SettingManager::~SettingManager()
{
}

void SettingManager::initialize(GMainLoop* mainloop)
{

}

int SettingManager::getLowEnter()
{
    return m_lowEnter;
}

int SettingManager::getLowExit()
{
    return m_lowExit;
}

int SettingManager::getCriticalEnter()
{
    return m_criticalEnter;
}

int SettingManager::getCriticalExit()
{
    return m_criticalExit;
}

int SettingManager::getDefaultRequiredMemory()
{
    return 120;
}

int SettingManager::getRetryCount()
{
    return 5;
}

bool SettingManager::isVerbose()
{
    return true;
}

