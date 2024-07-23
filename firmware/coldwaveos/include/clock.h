/*******************************************************************************
 * @file       clock.h
 * @brief
 * @copyright  Copyright (c) 2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @brief Clock driver API
 * minimum API for Clock icm20649.
 */

#ifndef EXCLUDE_FROM_DOCS
struct clock_driver
    {
    struct driver drv;
    };
#endif

/**
 * @defgroup drv_clock CLOCK
 * @brief
 * @details
 * @{
 */

/** @} */

/** @} */


#endif //_CLOCK_H_
