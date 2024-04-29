//
// Created by Sebastian Floss on 10.07.19.
// Copyright (c) 2019 ImagineOn GmbH. All rights reserved.
//

#ifndef _COLDWAVEOS_RANDOM_H
#define _COLDWAVEOS_RANDOM_H

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <driver.h>

#ifndef EXCLUDE_FROM_DOCS
struct rng_driver
	{
	struct driver drv;
	int (*rng_get_bytes) (struct device *, unsigned char *buf, size_t len);
	};
#endif

CW_DRIVER_FUNC
rng_get_bytes (int hDev, unsigned char *buf, size_t len)
CW_DRIVER_CALL(rng, get_bytes, hDev, buf, len);

int random_bytes (unsigned char *buf, size_t len);

#if __cplusplus
}
#endif

#endif //_COLDWAVEOS_RANDOM_H
