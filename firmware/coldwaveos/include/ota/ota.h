/*******************************************************************************
 * @file       ota.c
 * @brief
 * @copyright  Copyright (c) 2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef OTA_H
#define OTA_H

#include <inttypes.h>
#include <stdlib.h>
#include "checksum.h"

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @defgroup ota Firmware Updates
 * @brief Firmware Updater Functions
 * @details
 * @{
 */


#define OTA_MAX_UPDATE_TARGETS      (16)     ///< maximum number of external targets for firmware updates

typedef int (*ota_begin_t) (void *p);
typedef int (*ota_append_t) (void *p, uint8_t const *const data, size_t const size);
typedef int (*ota_finalize_t) (void *p);
typedef int (*ota_abort_t) (void *p);

typedef void *ota_context_t;

typedef struct ota_target_handler_t
	{
	ota_context_t ctx;
	ota_begin_t on_update_begin;    ///< callback to invoke when the update process begins
	ota_append_t on_update_append;   ///< callback to invoke when the update process requests more bytes
	ota_finalize_t on_update_finalize; ///< callback to invoke when the update process finishes
	ota_abort_t on_update_abort;    ///< callback to invoke when the update process was aborted
	} ota_target_handler_t;

typedef struct ota_image_version_t
	{
	uint8_t maj;
	uint8_t min;
	uint16_t patch;
	uint32_t build;
	} ota_image_version_t;

typedef enum ota_protocol_t
	{
	OTA_PROTO_TCP = 0,
	OTA_PROTO_TFTP
	} ota_protocol_t;

typedef struct ota_server_t
	{
	ota_protocol_t protocol;
	char *hostname;
	uint16_t port;
	} ota_server_t;

typedef struct ota_t
	{
	int target;
	ota_server_t server;
	uint32_t size_bytes;
	ota_checksum_algorithm_t cs_algo;
	ota_checksum_t cs;
	} ota_t;

#define OTA_FLG_IDLE                     (0)
#define OTA_FLG_SUCCEEDED                (1)     ///< a firmware update was downloaded and installed successfully
#define OTA_FLG_FAILED                   (2)     ///< the firmware update failed
#define OTA_FLG_FAILED_CONNECT           (5)     ///< a  connection to the update server could not be established
#define OTA_FLG_FAILED_UPDATE_RUNNING    (6)     ///< a firmware update was requested, while another update already was running
#define OTA_FLG_FAILED_FLASH             (7)     ///< an error occured during firmware update while writing to the flash
#define OTA_FLG_FAILED_IMAGE_TOO_BIG     (8)     ///< the update to be installed is too big for the available flash space
#define OTA_FLG_FAILED_UNSUP_ALGO        (9)     ///< the signature/checksum algorithm requested is not suported.
#define OTA_FLG_FAILED_CHECKSUM          (10)    ///< the firmware checksum didn't match the one provided with the update-request
#define OTA_FLG_FAILED_REMAINING_BYTES   (11)    ///< the download process finished before all expected bytes could be downloaded
#define OTA_FLG_FAILED_TIMEOUT           (12)    ///< a timeout occured while downloading the firmware update
#define OTA_FLG_FAILED_SIGNATURE         (13)    ///< the firmware wasn't signed by a trusted fota server
#define OTA_FLG_FAILED_TARGET            (14)    ///< the target isn't supported
#define OTA_FLG_FAILED_RESOLVE_HOST      (15)    ///< couldn't resolve hostname
#define OTA_FLG_FAILED_METADATA          (16)    ///< metadata is missing relevant information
#define OTA_FLG_UPDATE_STARTED           (20)    ///< a firmware update was started and is currently executing
#define OTA_FLG_PENDING_UPDATE           (0xAA)  ///< a firmware update was donwloaded but not yet installed or activated

///
/// \brief download and install a firmware update
/// \param flash_device flash device handle as obtained from a call to \ref open(const char*)
/// \param proto download update via raw TCP or TFTP/UDP
/// \param hostname hostname or ip-address (as string) of the update-server
/// \param port TCP port of the update server
/// \param use_tls Use a TLS Connection to the update server
/// \param server_cert the server's TLS certificate
/// \param server_cert_len the len of the server certificate in bytes
/// \param firmware_version firmware version index
/// \param firmware_version_str firmware version string (semver)
/// \param app_size size of the update in bytes
/// \param checksum checksum of the update
/// \param checksum_algorithm algorithm to be used for the checksum. currently only SHA1 is supported.
/// \param signature siganture of the update
/// \param signature_algorithm signture algorithm to be used for the checksum. currently only ECDSA-256 is supported.
/// \param update_target_id the id of the registered target to be updated
/// \return
int
ota_download (ota_t *ota);

int
ota_register_target_handler (int update_target_id, ota_target_handler_t *hnd);



/** @} */

#if defined (__cplusplus)
}
#endif

#endif // OTA_H

/** @} */