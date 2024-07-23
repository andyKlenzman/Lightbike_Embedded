/*******************************************************************************
 * @file       wlan.h
 * @brief
 * @copyright  Copyright (c) 2020-2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef _COLDWAVEOS_WLAN_H
#define _COLDWAVEOS_WLAN_H

#include <driver.h>
#include <inttypes.h>
#include <stdbool.h>

#if defined (__cplusplus)
extern "C" {
#endif



/**
 * @defgroup drv_networking_wlan Wireless LAN
 * \ingroup drv_networking
 * @brief Wireless LAN API
 * @details The implementation supports station- and access-point mode.
 * If they can both be used individually or even at the same time depends
 * on the underlying chipset.
 *
 *  Sysprops
 * --------
 * The Wireless LAN driver does not have any sysprops defined.
 *
 *
 * @{
 */

#define SSID_MAX_LEN  (34)

typedef enum wlan_ioctl
{

	//! Hostname
	WLAN_IOCTL_HOSTNAME = 0,
	//! ScanType
	WLAN_IOCTL_SCAN_TYPE

} wlan_ioctl_t;

typedef enum wlan_security_mode
{

	//! NO security
	WLAN_SEC_MODE_OPEN = 0,
	//! WEP Security with PSK
	WLAN_SEC_MODE_WEP,
    //! WPA security with PSK
    WLAN_SEC_MODE_WPA,
    //! WPA2 security with PSK
    WLAN_SEC_MODE_WPA2,
    //! WPA3 security with PSK
    WLAN_SEC_MODE_WPA3

} wlan_security_mode_t;

typedef enum wlan_connection_state {
    //! WLAN is currently not connected to an AP
    WLAN_STATE_DISCONNECTED = 0,
    //! WLAN is trying to connect to an AP
    WLAN_STATE_CONNECTING,
    //! WLAN is connected to an AP
    WLAN_STATE_CONNECTED
} wlan_connection_state_t;

typedef enum wlan_interface_type {
    //! WLAN station mode interface
    WLAN_STA_INTERFACE = 0,
    //! WLAN accespoint-mode interface
    WLAN_AP_INTERFACE
} wlan_interface_type_t;

typedef void (*wlan_status_handler_t) (int);

typedef struct wlan_ap_info {
    //! channel number of the scanned AP
    uint8_t rf_channel;
    //! security mode of the scanned AP
    wlan_security_mode_t security_mode;
    //! rssi value of the scanned AP
    int8_t rssi_val;
    //! network type of the scanned AP
    uint8_t network_type;
    //! SSID of the scanned AP
    uint8_t ssid[SSID_MAX_LEN];
    //! BSSID of the scanned AP
    uint8_t bssid[6];
    //! reserved
    uint8_t reserved[2];
} wlan_ap_info_t;

/**
 * @brief WLAN driver API
 * minimum API for WLAN icm20649.
 */
#ifndef EXCLUDE_FROM_DOCS
struct wlan_driver
    {
    struct driver drv;
    int (*wlan_connect) (struct device *, uint8_t *, uint8_t *, wlan_security_mode_t, void *, bool);
    int (*wlan_disconnect) (struct device *);
    int (*wlan_connection_state) (struct device *, wlan_connection_state_t *);
    int (*wlan_scan) (struct device *, uint8_t *, uint8_t, wlan_ap_info_t *, uint32_t *, uint32_t);
    // replaced by register_event
    int (*wlan_register_status_handler) (struct device *, wlan_status_handler_t);
    // replace by get/set option
    int (*wlan_get_hwaddr) (struct device *, uint8_t *, uint8_t, wlan_interface_type_t);
    int (*wlan_start_ap) (struct device *, uint16_t, uint8_t *, uint32_t,
                          uint8_t, wlan_security_mode_t, uint8_t *, uint16_t);
    int (*wlan_stop_ap) (struct device *);
    };
#endif

///
/// \brief registration of a callback that gets invoked when the connecton state changes.
/// \param hDev device handle as returned by \ref open(const char*)
/// \param handler handler function to be called upon connection state change.
/// \return 0 if the registration succeeded.
CW_DRIVER_FUNC wlan_register_status_handler (int hDev, wlan_status_handler_t handler)
CW_DRIVER_CALL(wlan, register_status_handler, hDev, handler) ;

///
/// \brief connects the WLAN subsystem to the AP with the specified ssid
/// \param hDev device handle as returned by \ref open(const char*)
/// \param ssid identifier of the WLAN AP to connect to. null-terminated string
/// \param bssid MAC address of the WLAN AP to connect to. needed for WPA3
/// \param sec_type \ref wlan_security_mode_t to use when connecting
/// \param secret_key the PSK to authenticate with. null-terminated string, max 63bytes (+ trailing \0)
/// \param synchronous true = wait for the connection process to complete or fail.
/// \return 0 on success.
CW_DRIVER_FUNC wlan_connect (int hDev, uint8_t *ssid, uint8_t *bssid, wlan_security_mode_t sec_type, void *secret_key, bool synchronous)
CW_DRIVER_CALL(wlan, connect, hDev, ssid, bssid, sec_type, secret_key, synchronous) ;

///
/// \brief disconnects the WLAN subsystem from an AP
/// \param hdev device handle as returned by \ref open(const char*)
CW_DRIVER_FUNC wlan_disconnect (int hDev)
CW_DRIVER_CALL_VOID(wlan, disconnect, hDev) ;

/**
 * @brief get the current state (connecting/connected/disconnected)
 * @param hdev device handle as returned by \ref open(const char*)
 * @return
 */
CW_DRIVER_FUNC wlan_connection_state (int hDev, wlan_connection_state_t *state)
CW_DRIVER_CALL(wlan, connection_state, hDev, state) ;

/**
 * @brief retrieve the WLAN hardware MAC address
 * @param hdev device handle as returned by \ref open(const char*)
 * @param addr caller-allocated buffer.
 * @param addr_len  length of the addr buffer. should be 6.
 * @param interface get the HW Addr of the STA or SOFTAP interface
 */
CW_DRIVER_FUNC wlan_get_hwaddr (int hDev, uint8_t *addr, uint8_t addr_len, wlan_interface_type_t interface)
CW_DRIVER_CALL(wlan, get_hwaddr, hDev, addr, addr_len, interface) ;

/**
 * @brief scans for available WLAN APs.
 * @details a differentiation beteween active or passive scanning can be configured
 *  via ioctl calls, if the HAL implementation supports it.
 * @param hdev device handle as returned by \ref open(const char*)
 * @param ssid pass an SSID here, if only one network should be scanned for.
 * @param chno the 2.4GHz channel number to scan. pass 0 to scan all channels
 * @param result array of \ref wlan_ap_info_t to hold the result of the scan. caller-allocated.
 * @param num_result number of results returned from the scan.
 * @param max_result maximum number of networks to return. has to match the size of the passed result-buffer.
 * @return 0 if the scan succeeded.
 */
CW_DRIVER_FUNC wlan_scan (int hDev, uint8_t *ssid, uint8_t chno, wlan_ap_info_t *result, uint32_t *num_result, uint32_t max_result)
CW_DRIVER_CALL(wlan, scan, hDev, ssid, chno, result, num_result, max_result) ;

/// \brief opens an access-point for external clients to connect to.
/// \details be aware, that most WLAN chips will only allow a single (or maybe a few) clients
/// to connect to an access point at the same time. This function therefore is mainly used as a means
/// for commissioning of the station-mode WLAN, i.e. to connect the device to an access point with internet connection.
/// \param hDev device handle as returned by \ref open(const char*)
/// \param channel the 2.4GHz channel the access-point should be started on
/// \param ssid the SSID the access point should broadcast
/// \param ssid_length the length of the SSID string
/// \param hide_ssid if no SSID should be broadcast, but instead the AP should only be discoverable via BSSID, set to true
/// \param sec_type the \ref wlan_security_mode_t security type the access-point should employ
/// \param passkey the PSK for connecting to the AP.
/// \param passkey_length the length of the PSK.
/// \return 0 if the access point was successfully started, error-code otherwise.
CW_DRIVER_FUNC wlan_start_ap (int hDev, uint16_t channel, uint8_t *ssid, uint32_t ssid_length,
                              uint8_t hide_ssid, wlan_security_mode_t sec_type, uint8_t *passkey, uint16_t passkey_length)
CW_DRIVER_CALL(wlan, start_ap, hDev, channel, ssid, ssid_length, hide_ssid, sec_type, passkey, passkey_length) ;

/// \brief close a previously started access-point.
/// \param hDev device handle as returned by \ref open(const char*)
/// \return 0 if the access point was successfully started, error-code otherwise.
CW_DRIVER_FUNC wlan_stop_ap (int hDev)
CW_DRIVER_CALL_VOID(wlan, stop_ap, hDev) ;



/**  @} */
 

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_WLAN_H
