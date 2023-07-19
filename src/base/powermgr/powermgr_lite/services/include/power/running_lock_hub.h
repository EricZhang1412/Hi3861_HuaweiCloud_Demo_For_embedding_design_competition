/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef POWERMGR_RUNNING_LOCK_HUB_H
#define POWERMGR_RUNNING_LOCK_HUB_H

#include <ohos_types.h>

#include "suspend_ops.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

BOOL RunningLockHubInit(struct AutoSuspendOps* suspendOps);
void RunningLockHubLock(const char* name);
void RunningLockHubUnlock(const char* name);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // POWERMGR_RUNNING_LOCK_HUB_H
