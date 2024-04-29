//
// Created by Sebastian Floss on 11.06.21.
// Copyright (c) 2021 ImagineOn GmbH. All rights reserved.
//

#ifndef _ETHERNETIF_H_
#define _ETHERNETIF_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


typedef void *ethernetif;

typedef int8_t (*ethernetif_linkoutput_fn) (ethernetif, unsigned char *buf, unsigned int len);

void
ethernetif_input (ethernetif eth_if, unsigned char *buf, unsigned int buflen);

ethernetif
ethernetif_add (ethernetif_linkoutput_fn output_fn, const char *hostname, uint16_t mtu);

int8_t
ethernetif_remove (ethernetif);

void
ethernetif_hwaddr (ethernetif eth_if, unsigned char *hwaddr, unsigned int hwaddr_len);

int8_t
ethernetif_set_hwaddr (ethernetif eth_if, unsigned char *hwaddr, unsigned int hwaddr_len);

ethernetif
ethernetif_default ();

int8_t
ethernetif_set_default (ethernetif);

int8_t
ethernetif_set_up (ethernetif);

int8_t
ethernetif_ip (ethernetif eth_if, uint32_t * ip_addr, uint32_t * gw, uint32_t * netmask);

int8_t
ethernetif_set_ip(ethernetif eth_if, uint32_t * ip_addr, uint32_t * gw, uint32_t * netmask);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_ETHERNETIF_H_
