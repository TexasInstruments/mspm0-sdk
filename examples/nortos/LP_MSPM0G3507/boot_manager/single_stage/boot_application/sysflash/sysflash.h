/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef __SYSFLASH_H__
#define __SYSFLASH_H__

#include "mcuboot_config/mcuboot_config.h"

#define FLASH_AREA_BOOTLOADER 0
#define FLASH_AREA_IMAGE_0 1
#define FLASH_AREA_IMAGE_1 2
#define FLASH_AREA_IMAGE_SCRATCH 3
#define FLASH_AREA_IMAGE_2 5
#define FLASH_AREA_IMAGE_3 6

#if (MCUBOOT_IMAGE_NUMBER == 1)
#define FLASH_AREA_IMAGE_PRIMARY(x) \
    (((x) == 0) ? FLASH_AREA_IMAGE_0 : FLASH_AREA_IMAGE_0)
#define FLASH_AREA_IMAGE_SECONDARY(x) \
    (((x) == 0) ? FLASH_AREA_IMAGE_1 : FLASH_AREA_IMAGE_1)

#elif (MCUBOOT_IMAGE_NUMBER == 2)

#define FLASH_AREA_IMAGE_PRIMARY(x) \
    (((x) == 0) ? FLASH_AREA_IMAGE_0 : ((x) == 1) ? FLASH_AREA_IMAGE_2 : 255)
#define FLASH_AREA_IMAGE_SECONDARY(x) \
    (((x) == 0) ? FLASH_AREA_IMAGE_1 : ((x) == 1) ? FLASH_AREA_IMAGE_3 : 255)

#else
#warning "Image slot and flash area mapping is not defined"
#endif

#endif /* __SYSFLASH_H__ */
