/*******************************************************************************
 * @file       checksum.h
 * @brief
 * @copyright  Copyright (c) 2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/


#ifndef OTA_CHECKSUM_H
#define OTA_CHECKSUM_H

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define OTA_CHECKSUM_NO_MATCH          (-3)
#define OTA_CHECKSUM_UNSUPPORTED_ALGO  (-1)
#define OTA_CHECKSUM_ALGO_OK           ( 0)
#define OTA_CHECKSUM_MATCH             ( 0)

/**
 The maximum size of a checksum,
 for any algorithm currently used or that might be used in the
 foreseeable future.
 */
#define OTA_CHECKSUM_VALUE_SIZE_MAX (64)

/**
 A checksum value type, valid for any supported algorithm.
 Algorithms with shorter checksums will simply
 ignore a number of entries at the back of the array.
 */
typedef uint8_t ota_checksum_t[OTA_CHECKSUM_VALUE_SIZE_MAX];

/**
 The maximum length of a string representing the name of a checksum-algorithm
 (as given by `strlen`, i.e. not counting the terminating '\0').
 */
#define OTA_CHECKSUM_ALGORITHM_NAME_STRLEN_MAX (15)

/**
 A fixed-size string type for checksum-algorithm names.
 It is safer to identify algorithms by name than with an enmeration
 so we have backwards compatibility between different versions of this
 library without worrying about preserving and
 documenting the value of each enum case.
 */
typedef char ota_checksum_algorithm_t[OTA_CHECKSUM_ALGORITHM_NAME_STRLEN_MAX + 1];

int8_t
ota_checksum_algorithm_supported (ota_checksum_algorithm_t const algorithm);

uint8_t
ota_checksum_algorithm_digest_length (ota_checksum_algorithm_t const algorithm);

int8_t
ota_checksum_match (const unsigned char *data, size_t datalen,
					ota_checksum_algorithm_t const algorithm, const ota_checksum_t cs);


/**
   BEGIN: SHA-1 support
**/

/**
 Identifier string for algorithm SHA-1.
 */
#define OTA_CHECKSUM_ALGORITHM_SHA1  "SHA1"

/**
 Actual length of a checksum returned from the SHA-1 algorithm. These many bytes at the beginning
 of the `ota_checksum_t` array are used, while the remaining bytes are ignored.
 */

#define  OTA_SHA1_DIGEST_LENGTH  (20)

/**
 It is your responsibility to implement this function, which should compute the SHA-1 hash of the data
 written in flash memory from address `begin` to address `begin + size` (not included). The resulting
 hash should be written at the beginning of the `out` array.

 The reason why no default implementation is given with this library is because SHA-1 can often be computed
 in-place with a very efficient hardware implementation, in which case `ota_sha1` can be written as a
 simple wrapper for the corresponding interface.
 */
void ota_sha1 (ota_checksum_t out, char *begin, size_t size);


/**
     END: SHA-1 support
**/


#if defined (__cplusplus)
}
#endif

#endif /* OTA_CHECKSUM_H */
