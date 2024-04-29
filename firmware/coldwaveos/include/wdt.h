/********************************************************************************
  * @file    wdt.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   Watchdog HAL module driver.
  ********************************************************************************/


#ifndef COLDWAVEOS_WDT_H_
#define COLDWAVEOS_WDT_H_

#include <inttypes.h>
#include <stdbool.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define OS_WATCHDOG_PERIOD_DEFAULT_MS    (10000)

/**
 * @defgroup drv_wdt  Watchdog Timer
 * @brief Watchdog Timer Driver
 * @{
 */

/**
 * @brief ADWDTC driver API
 * minimum API for WDT drivers.
 */
#ifndef EXCLUDE_FROM_DOCS
struct wdt_driver
    {
    struct driver drv;
    cw_driver_return_t (*wdt_enable) (struct device *);
    cw_driver_return_t (*wdt_disable) (struct device *);
    cw_driver_return_t (*wdt_reset) (struct device *);
    };
#endif

/**
 * @brief Disable the watchdog timer
 */
CW_DRIVER_FUNC
wdt_disable (int hDev)
CW_DRIVER_CALL_VOID(wdt, disable, hDev);

/**
 * @brief Enable the watchdog timer
 */
CW_DRIVER_FUNC
wdt_enable (int hDev)
CW_DRIVER_CALL_VOID(wdt, enable, hDev);
/**
 * @brief Reset the watchdog timer
 */
CW_DRIVER_FUNC
wdt_reset (int hDev)
CW_DRIVER_CALL_VOID(wdt, reset, hDev);


/**  @} */


#if defined (__cplusplus)
}
#endif

#endif //COLDWAVEOS_WDT_H_
