//
// Copyright (c) 2024 ImagineOn GmbH. All rights reserved.
//

#ifndef UTIL_CLI_COMMAND_DEFINITIONS_H
#define UTIL_CLI_COMMAND_DEFINITIONS_H

#define CLI_INFO                    "info"
#define CLI_RESET                   "reset"

/// VirtualGear API
#define CLI_VGEAR_HELP               "vgear-help"
#define CLI_VGEAR_INFO               "vgear-info"
#define CLI_VGEAR_STATUS             "vgear-status"
#define CLI_VGEAR_GET                "vgear-get"
#define CLI_VGEAR_SET                "vgear-set"
#define CLI_VGEAR_SET_POWER          "vgear-set-power"
#define CLI_VGEAR_SET_XY             "vgear-set-xy"
#define CLI_VGEAR_SET_TC             "vgear-set-tc"
#define CLI_VGEAR_SET_RGBW           "vgear-set-rgbw"
#define CLI_VGEAR_GROUP_ADD          "vgear-group-add"
#define CLI_VGEAR_GROUP_REMOVE       "vgear-group-remove"
#define CLI_VGEAR_SCENE_ADD          "vgear-scene-add"
#define CLI_VGEAR_SCENE_REMOVE       "vgear-scene-remove"
#define CLI_VGEAR_SCENE_RECALL       "vgear-scene-recall"

/// VirtualBus API
#define CLI_VBUS_INIT               "vbus-init"
#define CLI_VBUS_GET_MODE           "vbus-mode"
#define CLI_VBUS_GET_STATUS         "vbus-status"
#define CLI_VBUS_ADD_DEVICES        "vbus-add-devices"
#define CLI_VBUS_CLEAR_DEVICE       "vbus-clear"
#define CLI_VBUS_CLEAR_ALL          "vbus-clear-all"
#define CLI_VBUS_SEND_RAW           "vbus-raw"

// DALI
#define CLI_DALI_DAPC               "dali-dapc"
#define CLI_DALI_QUERY              "dali-query"
#define CLI_DALI_QUERY_ACTUAL_LEVEL "dali-query-actual-level"

#endif //UTIL_CLI_COMMAND_DEFINITIONS_H
