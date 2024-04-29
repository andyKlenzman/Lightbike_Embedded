//
// Created by Sebastian Floss on 25.09.18.
// Copyright (c) 2018 ImagineOn GmbH. All rights reserved.
//

#ifndef _COLDWAVEOS_FLASH_H
#define _COLDWAVEOS_FLASH_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
* @defgroup drv_flash Flash memory read / write
* \ingroup drv
* @brief
* @details
* @{
*/

typedef struct flash_sector_layout_t
    {
    uint32_t nSectors;
    uint32_t sectorSize;
    } flash_sector_layout_t; //name here allows you to create variables of this type w out using struct keyword each time

typedef struct flash_info_t
    {
    uint32_t startAddress;
    uint32_t totalSize;
    uint32_t writeSize;
    uint32_t nBanks: 2;
    uint32_t bankSize: 28;
    uint32_t banksSwitchable: 1;
    uint32_t banksSwitched: 1;
    uint32_t nSectorLayouts;
    const flash_sector_layout_t *sectorLayouts;
    } flash_info_t;

#ifndef EXCLUDE_FROM_DOCS
struct flash_driver
    {
    struct driver drv;
    int (*flash_info) (struct device *, flash_info_t *);
    int (*flash_erase) (struct device *, uint32_t sector_address);
    int (*flash_write) (struct device *, uint32_t address, unsigned char *data, size_t len);
    };
#endif

///

CW_DRIVER_FUNC
flash_info (int hDev, flash_info_t *info)
CW_DRIVER_CALL(flash, info, hDev, info);

///
/// \param hDev
/// \param sector_address
/// \return
CW_DRIVER_FUNC
flash_erase (int hDev, uint32_t sector_address)
CW_DRIVER_CALL(flash, erase, hDev, sector_address);

///
/// \param hDev
/// \param address
/// \param data
/// \param len
/// \return
CW_DRIVER_FUNC
flash_write (int hDev, uint32_t address, unsigned char *data, size_t len)
CW_DRIVER_CALL (flash, write, hDev, address, data, len);


/** @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_FLASH_H
