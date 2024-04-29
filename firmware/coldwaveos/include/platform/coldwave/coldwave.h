//
// Created by Sebastian Floss on 13.02.22.
// Copyright (c) 2022 ImagineOn GmbH. All rights reserved.
//

#ifndef _COLDWAVE_H_
#define _COLDWAVE_H_

#include <kernel.h>
#include <flake/flake.h>
#include "commissioning/wifi_commissioning.h"


/**
 * @defgroup coldwave Coldwave
 * @brief Coldwave API
 * @details

 * @{
 */

#define CW_PTP                      (0)
#define CW_NETWORK                  (1)

typedef int  (*coldwave_auth_callback_t)(const flake::PropArray& rops);

typedef struct coldwave_options_t {
    int no_local_tls;                       ///< if set, local connection ("Broadband" only) will be unencrypted
    uint16_t local_tcp_port;                ///< if set to 0 the default port (9986) will be used
    coldwave_auth_callback_t auth_callback; ///< called on every new local connection ("Broadband") only
    int auto_update_disabled;               ///< if set, the CBE will not start OTA when new versions are available
} coldwave_options_t;

typedef struct coldwave_init_t {
    uint8_t networking_bw;                  ///< Narrowband or Broadband connectivity
    const char* app_semver;                 ///< SemVer Version String of current firmware app
    const char* device_id;                  ///< unique identifier for the device
    const char* product_id;                 ///< Coldwave Product Identifier (4 alphanumeric characters)
    const char* hw_id;                      ///< Hardware (SoC) Identifier for Updates
    coldwave_options_t opts;
} coldwave_init_t;

int
coldwave_init (coldwave_init_t* init, const char* srv_uuid, flake::Service** srv);

int
coldwave_backend_attach (const char *cbe_fqdn, unsigned char *cbe_ca_cert, unsigned cbe_ca_cert_len);

int
coldwave_backend_detach (void);

int
coldwave_deinit ();

int
coldwave_start_ap_wifi_commissioning(int wifi_dev, const char* ssid, const char* pass, uint32_t timeout_ms,
                                          wifi_commissioning_data_callback_t cb);

int
coldwave_start_ble_wifi_commissioning(int wifi_dev, int ble_dev, const char* ble_name, uint32_t timeout_ms,
                                           wifi_commissioning_data_callback_t cb);

int
coldwave_start_legacy_wifi_commissioning(int wifi_dev, int ble_dev, const char* ble_name, uint32_t timeout_ms,
                                       wifi_commissioning_data_callback_t cb);


void
coldwave_end_wifi_commissioning (void);

int
coldwave_ota_autoupdate_enable (bool e);

bool
coldwave_ota_autoupdate_enabled (void);

/** }@ **/

#endif //_COLDWAVE_H_
