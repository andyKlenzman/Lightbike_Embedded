//
// Created by Sebastian Floss on 11.06.21.
// Copyright (c) 2021 ImagineOn GmbH. All rights reserved.
//

#ifndef _ppposif_H_
#define _ppposif_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef void *ppposif;

typedef int8_t (*ppposif_output_fn) (ppposif, unsigned char *buf, unsigned int len);
typedef void (*ppposif_status_fn )(ppposif, int err_code);

void
ppposif_input (ppposif ppp_if, unsigned char *buf, unsigned int buflen);

ppposif
ppposif_add (ppposif_output_fn output_fn, ppposif_status_fn status_fn, void* ctx);

int8_t
ppposif_remove (ppposif);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_ppposif_H_
