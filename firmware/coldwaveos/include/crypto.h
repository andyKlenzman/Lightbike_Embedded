//
// Created by Sebastian Floss on 31.05.19.
// Copyright (c) 2019 ImagineOn GmbH. All rights reserved.
//

#ifndef _COLDWAVEOS_CRYPTO_H
#define _COLDWAVEOS_CRYPTO_H

#include <inttypes.h>
#include <driver.h>
#include <psa/crypto.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @brief PSA driver API
 * minimum API for PSA icm20649.
 */
#ifndef EXCLUDE_FROM_DOCS
struct psa_driver
    {
    struct driver drv;
    };
#endif


#if defined (__cplusplus)
}
#endif


#endif //_COLDWAVEOS_CRYPTO_H
