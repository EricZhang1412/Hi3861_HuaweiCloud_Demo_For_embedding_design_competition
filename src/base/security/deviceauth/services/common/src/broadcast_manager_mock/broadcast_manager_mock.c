/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "broadcast_manager.h"
#include "device_auth_defines.h"
#include "hc_log.h"

bool IsBroadcastSupported()
{
    return false;
}

int32_t InitBroadcastManager()
{
    return HC_SUCCESS;
}

void DestroyBroadcastManager()
{
    return;
}

Broadcaster *GetBroadcaster()
{
    return NULL;
}

int32_t AddListener(const char *appId, const DataChangeListener *listener)
{
    (void)appId;
    (void)listener;
    LOGE("Broadcast manager is not supported!");
    return HC_ERR_NOT_SUPPORT;
}

int32_t RemoveListener(const char *appId)
{
    (void)appId;
    LOGE("Broadcast manager is not supported!");
    return HC_ERR_NOT_SUPPORT;
}
