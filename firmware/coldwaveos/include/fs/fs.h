//
// Created by Sebastian Floss on 25.08.21.
// Copyright (c) 2021 ImagineOn GmbH. All rights reserved.
//

#ifndef _FS_H_
#define _FS_H_

#include <stddef.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup drv_fs FileSystem
/// @{
///

#define E_FS_OK               (0)
#define E_FS_ENTRIES_DIFFER   (-1)
#define E_FS_NOT_FOUND        (-2)
#define E_FS_PARAM            (-3)
#define E_FS_KEY              (-4)
#define E_FS_SIZE             (-5)
#define E_FS_ALLOC            (-6)
#define E_FS_RAM              (-7)
#define E_FS_CHECKSUM         (-8)
#define E_FS_FLASH            (-9)

/// @brief initialize the key-value-storage filesystem
/// @details the filesystem always resides at the end of the available flash memory
///     the size gets defined by the linker symbol `__fs_reserved_space` which in
///     CMake builds can be set via the variable FILESYSTEM_SIZE_BYTES.
///     if the flash has two switchable banks, the size will be doubled, by using banks.
///     Therefore it is vital that the HAL flash configuration correctly defines
///     FLASH_LENGTH as half the total flash size, i.e. the size of a single bank.sa
///     The number of individual store operations before wearout will approximately:
///         (FS_SIZE / (20 + AVG_ITEM_SIZE)) * 10000.
///     For 16-byte values on a 64kB FS with 256B Pages this would be 18.2 million store operations,
///     which translates to one write every 20 seconds over ~12 years
/// \param dev handle to a flash device
/// \param start_address  if 0 is passed, the flash driver uses the first available flash address
/// \param size_bytes  if 0 is passed, the fs uses the maximum available space.
/// \param auto_repack pass 1 if the filesystem should check and perform repacking on every write automatically
/// \return handle for using the filesystem
void *fs_init_ex (int dev, uint32_t start_address, uint32_t size_bytes, int auto_repack);

void *fs_init (void);

void fs_deinit (void *hdn);

void fs_erase_all (void *hnd);

int fs_get (void *hnd, uint16_t key, unsigned char *data, uint16_t max_len, uint16_t *len);

int fs_set (void *hnd, uint16_t key, unsigned char *data, uint16_t len);

uint16_t fs_num_keys (void *hnd);

uint16_t fs_key_at (void* hnd, uint16_t key);

int fs_available_memory (void* hnd);

int fs_freeable_memory (void* hnd);

int fs_repack(void *hnd);

int fs_remove (void* hnd, uint16_t key);

int fs_wear_level (void* hnd);


/** @} */

#ifdef __cplusplus
}
#endif

#endif //_FS_H_
