/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BASE_STARTUP_INITLITE_DEVICE_H
#define BASE_STARTUP_INITLITE_DEVICE_H

#include <sys/types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DEV_KMSG_MINOR 11
#define DEV_NULL_MINOR 3
#define DEV_RANDOM_MINOR 8
#define DEV_URANDOM_MINOR 9

void MountBasicFs(void);
void CreateDeviceNode(void);
int MakeSocketDir(const char *path, mode_t mode);
void CloseStdio(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif // BASE_STARTUP_INITLITE_DEVICE_H
