/********************************************************************************
  * @file    ble_gatt.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   BLE GATT HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_BLE_MANAGER_H
#define _COLDWAVEOS_BLE_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <kernel.h>

#define  BLE_ATT_PROPERTY_READ      0x02
#define  BLE_ATT_PROPERTY_WRITE     0x08
#define  BLE_ATT_PROPERTY_NOTIFY    0x10
#define  BLE_ATT_PROPERTY_INDICATE  0x20

#if defined (__cplusplus)
extern "C" {
#endif

/**
* @addtogroup drv_ble
* @{
*/

/**
 * @brief Structure to convey information about UUIDs in BLE.
 * @details By definition BLE UUIDs are always 'reversed'.
 * For example the following UUID { 77880001-d229-11e4-8689-0004a5d5c51b }
 * would be represented in ble_uuid128_t format as follows:
 * \code{.c}
 * ble_uuid128_t my_uuid = {
 *    .data1 = {0x1b, 0xc5, 0xd5, 0xa5},
 *    .data2 = {0x04, 0x00},
 *    .data3 = {0x89, 0x86},
 *    .data4 = {0xe4, 0x11, 0x29, 0xd2, 0x01, 0x00, 0x88, 0x77}
 *  }
 * \endcode
 */
typedef struct ble_uuid128_t {
    uint8_t data1[4]; /// last 4 bytes of the UUID in reversed byte order
    uint8_t data2[2]; /// 11th and 12th byte of the UUID in reversed byte order
    uint8_t data3[2]; /// 9th and 10th byte of the UUID in reversed byte order
    uint8_t data4[8]; /// first 8 bytes of the UUID in reversed byte order
} ble_uuid128_t;


/// callback-prototype for peripheral-connection events
typedef void (*ble_manager_on_connect) (uint16_t, void *);
/// callback-prototype for peripheral-disconnection events
typedef void (*ble_manager_on_disconnect) (uint16_t, void *);
/// callback-prototype for peripheral-read events
typedef int  (*ble_manager_on_read) (uint16_t, uint16_t, uint8_t *, uint16_t, void *);
/// callback-prototype for peripheral-write events
typedef void (*ble_manager_on_write) (uint16_t, uint16_t, uint8_t *, uint16_t, bool, void *);

/**
 * @brief Structure returned by the BLE driver to configure and interact with the peripheral
 */
struct ble_gatt {

    /// Initialize the device for use.
    /// Needs to be called before any other functions are called.
    /// \param ble_mgr_on_connect_cb  function to be called when a peripheral connects
    /// \param ble_mgr_on_disconnect_cb function to be called when a peripheral disconnects
    /// \param ble_mgr_on_write_cb function to be called when a peripheral writes data
    /// \param ble_mgr_on_read_cb function to be called when a peripheral wants to read data
    /// \param context user data pointer that will be passed to all callback functions. can be used to identify the instance, e.g.
    /// \param capabilities reserved, set this to zero.
    void (*init) (
        ble_manager_on_connect ble_mgr_on_connect_cb,
        ble_manager_on_disconnect ble_mgr_on_disconnect_cb,
        ble_manager_on_write ble_mgr_on_write_cb,
        ble_manager_on_read ble_mgr_on_read_cb,
        void *context,
        uint32_t capabilities);

    /// Free the resources taken by the Gatt-instance. Should be called before closing the driver handle
    void (*deinit) (void);

    /// Some BLE drivers need some time to free up all resources.
    /// Since \ref deinit(void) is non-blocking, the successful finalization of the deinit process can
    /// be checked by calling this function.
    /// \return true if deinitialization has finished.
    bool (*uninitialized) (void);

    /// Begin the declaration of a GATT-Service.
    /// \param uuid Unique Idenitifier of the GATT Service.
    /// \return service-handle for referencing the service in read/write callbacks
    int32_t (*begin_service) (ble_uuid128_t uuid);

    /// End the declaration of a GATT-Service
    /// Can only be called after a previous call to \ref begin_service (ble_uuid128_t uuid)
    void (*end_service) (void);

    ///
    /// \param uuid
    /// \param modifiers
    /// \param len
    /// \return
    int32_t (*add_characteristic) (ble_uuid128_t uuid, uint8_t modifiers, uint16_t len);
    ///
    /// \param uuid
    /// \param modifiers
    /// \param bytes
    /// \param num_bytes
    /// \return
    int32_t (*add_descriptor) (ble_uuid128_t uuid, uint8_t modifiers, uint8_t *bytes, uint16_t num_bytes);

    /// Sets the descriptive name the central will be add to its advertising name.
    /// the name can't be longer than 29 bytes and should be kept short, commonly not exceed 10bytes.
    /// \param name the central's descriptive name as a null-terminated char array.
    /// \return 0 in case the call succeeded.
    int32_t (*set_local_name) (char *name);

    ///
    /// \param data
    /// \param len
    /// \return
    int32_t (*set_advertise_data) (uint8_t *data, uint8_t len);

    ///
    /// \param min
    /// \param max
    /// \return
    int32_t (*set_advertise_timing) (uint16_t min, uint16_t max);

    ///
    /// \param handle
    /// \param addr_type
    /// \param adress
    /// \param addr_len
    /// \return
    int32_t (*set_device_address) (uint8_t handle, uint8_t addr_type, uint8_t *adress, uint8_t addr_len);

    ///
    /// \param data
    /// \param len
    /// \return
    int32_t (*set_scan_response_data) (uint8_t *data, uint8_t len);

    /// Starts the previously configured advertising
    /// \return 0 in case of a successful execution.
    int32_t (*start_advertising) (void);

    /// Stops any currently running BLE advertising
    /// \return 0 in case of a successful execution.
    int32_t (*stop_advertising) (void);

    /// Disconnect any currently connected peripheral
    /// \return 0 in case of a successful disconnect.
    int32_t (*disconnect) (void);

    ///
    /// \param start_handle
    /// \param bytes
    /// \param num_bytes
    /// \return
    int32_t (*find_attribute_handle) (uint16_t start_handle, uint8_t *bytes, uint16_t num_bytes);

    ///
    /// \param handle
    /// \param bytes
    /// \param max_bytes
    /// \param num_bytes
    /// \return
    int32_t (*get_attribute_type) (uint16_t handle, uint8_t *bytes, uint16_t max_bytes, uint16_t *num_bytes);

    /// Get the currently active (negotiated) MTU for a connection in bytes.
    /// \return the currently active (negotiated) MTU for a connection in bytes or the drivers default MTU if
    /// there is no active connection.
    uint16_t (*get_mtu) (void);

    ///
    /// \param handle
    /// \param data
    /// \param len
    /// \param indicate
    /// \return
    int32_t (*notify) (uint16_t handle, uint8_t *data, uint8_t len, bool indicate);
};

/** @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_BLE_MANAGER_H
