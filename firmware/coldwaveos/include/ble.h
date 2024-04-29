/*******************************************************************************
 * @file     ble.h
 * @brief
 * @copyright  Copyright (c) 2020-2023. ImagineOn GmbH. www.imagineon.de
 * All rights reserved.
 ******************************************************************************/


#ifndef _COLDWAVEOS_BLE_H
#define _COLDWAVEOS_BLE_H

#include <inttypes.h>
#include <driver.h>
#include <ble/ble_gatt.h>

#if defined (__cplusplus)
extern "C" {
#endif


/**
 * @defgroup drv_ble Bluetooth LE
 * @brief Bluetooth LE Interface
 * @details The Bluetooth LE interface provides a means of communicating with Bluetooth LE devices, acting as Central
 *
 * Sysprops
 * --------
 * The Bluetooth LE driver does not have any sysprops defined.
 *
 * @{
 */

#define   BLE_IOCTL_SET_CONNECT_CALLBACK       (0x0001) ///< ioctl to register a function to be called on an incoming connection from a BLE peripheral
#define   BLE_IOCTL_SET_DISCONNECT_CALLBACK    (0x0002) ///< ioctl to register a function to be called on disconection of a BLE peripheral
#define   BLE_IOCTL_SET_LOCAL_NAME             (0x0003) ///< ioctl set a local name for the central to be advertised as
#define   BLE_IOCTL_DISCONNECT                 (0x0004) ///< ioctl to disconnect any connected peripheral
#define   BLE_IOCTL_START_ADVERTISING          (0x0005) ///< ioctl to start advertising
#define   BLE_IOCTL_STOP_ADVERTISING           (0x0006) ///< ioctl to stop advertising
#define   BLE_IOCTL_AUTO_RECONNECT             (0x0007) ///< ioctl to enable/disable auto-reconnect, i.e. if advertising should be auto re-started when a connection ends

#define   BLE_IOCTL_START_CW_TEST              (0x0F01) ///< ioctl to start compliance measurements (have the RF send a CW signal)
#define   BLE_IOCTL_STOP_CW_TEST               (0x0F02) ///< ioctl to stop CW mode

#ifndef EXCLUDE_FROM_DOCS
struct ble_driver
    {
    struct driver drv;
    int (*ble_get_gatt) (struct device *, struct ble_gatt **);

    };
#endif

/// 
/// \param hDev a device handle, returned by a prior call to \ref open(const char*).
/// \return 0 if successful, error code otherwise.
CW_DRIVER_FUNC ble_get_gatt (int hDev, struct ble_gatt **gatt_if)
CW_DRIVER_CALL(ble, get_gatt, hDev, gatt_if);

/** @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_BLE_H
