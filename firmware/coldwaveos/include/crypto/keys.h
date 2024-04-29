/*******************************************************************************
 * @file       keys.h
 * @license    This file is part of the ImagineOn Coldwave eOS software package
 *             licensed under the ImagineOn software-licensing terms available
 *             under https://www.imagineon.de/de/info/licensing-terms
 * @copyright  Copyright (c) 2023. ImagineOn GmbH. www.imagineon.de.
 ******************************************************************************/

#ifndef _KEYS_H_
#define _KEYS_H_

#include <crypto/psa/crypto.h>

#ifndef MBEDTLS_PSA_KEY_SLOT_COUNT
#define MBEDTLS_PSA_KEY_SLOT_COUNT  (32)
#endif

extern psa_key_id_t cw_crypto_key_id[MBEDTLS_PSA_KEY_SLOT_COUNT];

#define CRYPTO_KEY_ID_DEVICE_ED25519      (0)
#define CRYPTO_KEY_ID_DEVICE_SECP256R1    (1)
#define CRYPTO_KEY_ID_OTA_SECP256R1       (2)

#if defined (__cplusplus)
extern "C" {
#endif

static inline
psa_key_id_t crypto_key_id_deviceKeyED25519 (void)
  {
  return cw_crypto_key_id[CRYPTO_KEY_ID_DEVICE_ED25519];
  }

static inline
psa_key_id_t crypto_key_id_deviceKeySEPC256R1 (void)
  {
  return cw_crypto_key_id[CRYPTO_KEY_ID_DEVICE_SECP256R1];
  }

static inline
psa_key_id_t crypto_key_id_otaKeySECP256R1 (void)
  {
  return cw_crypto_key_id[CRYPTO_KEY_ID_OTA_SECP256R1];
  }

#if defined (__cplusplus)
}
#endif

#endif //_KEYS_H_
