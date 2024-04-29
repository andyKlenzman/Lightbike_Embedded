//
// Created by Sebastian Floss on 26.02.23.
// Copyright (c) 2023 ImagineOn GmbH. All rights reserved.
//

#ifndef _WIFI_COMMISSIONING_H_
#define _WIFI_COMMISSIONING_H_

/*! CPP guard */

#include <inttypes.h>
#include <flake/flake.h>
#include <wlan.h>

using namespace flake;

#define E_OK      (0)
#define E_FAILED  (-1)
#define E_RUNNING (-2)
#define E_SCAN    (-3)
#define E_TIMEOUT (-101)

#define WCT_BLE    (0)
#define WCT_AP     (1)
#define WCT_LEGACY (2)

#define WIFI_MAX_SCAN_RESULTS    (25)

#define WIFI_CAP_JOIN            (1)
#define WIFI_CAP_SCAN            (2)

#define CW_WIFI_DISCONNECTED     (0)
#define CW_WIFI_CONNECTING       (1)
#define CW_WIFI_CONNECTED        (2)

#define P_WIFI_IS_SCANNING       (uint32_t)((0x4000 << 16)     | TT_BOOL    | TAG_READONLY)
#define P_WIFI_CAPABILITIES      (uint32_t)((0x4001 << 16)     | TT_UINT8   | TAG_READONLY)
#define P_WIFI_CURR_SSID         (uint32_t)((0x4002 << 16)     | TT_STRING  | TAG_READONLY)
#define P_WIFI_CURR_BSSID        (uint32_t)((0x4003 << 16)     | TT_BIN  | TAG_READONLY)
#define P_WIFI_STATE             (uint32_t)((0x4004 << 16)     | TT_UINT8   | TAG_READONLY)
#define P_WIFI_IF_IP             (uint32_t)((0x4005 << 16)     | TT_UINT32  | TAG_READONLY)

//// Wifi Provisioning
#define P_ACTIONTOKEN            (uint32_t)((0x1000 << 16)     | TT_UINT16   | TAG_READONLY)
#define P_RESULT                 (uint32_t)((0x1001 << 16)     | TT_INT8    | TAG_READONLY)
#define P_SSID                   (uint32_t)((0x1001 << 16)     | TT_STRING  | TAG_ACTIONABLE)
#define P_BSSID                  (uint32_t)((0x1002 << 16)     | TT_BIN  | TAG_ACTIONABLE)
#define P_SECTYPE                (uint32_t)((0x1003 << 16)     | TT_UINT8   | TAG_ACTIONABLE)
#define P_PASSKEY                (uint32_t)((0x1004 << 16)     | TT_STRING  | TAG_ACTIONABLE)
#define P_RSSI                   (uint32_t)((0x1004 << 16)     | TT_INT16  | TAG_READONLY)


typedef struct wifi_commissioning_data_t {
    uint8_t sec_type;
    uint8_t ssid_length;
    uint8_t ssid[32];
    uint8_t passphrase_length;
    uint8_t passphrase[63];
    uint8_t bssid[6];
} wifi_commissioning_data_t;

typedef void  (*wifi_commissioning_data_callback_t)(int result, wifi_commissioning_data_t* data);

typedef struct wifi_commissioning_init_t {
    int wifi_dev;
    int commissioner_dev;
    const char* commissioner_name;
    const char* commissioner_password;
    Service *device_srv;
    uint32_t timeout_ms;
    wifi_commissioning_data_callback_t data_callback;
} wifi_commissioning_init_t;

int
wifi_commissioning_start (int wifi_commissioning_type, wifi_commissioning_init_t *init);

void
wifi_commissioning_end (void *p);

#endif //_WIFI_COMMISSIONING_H_
