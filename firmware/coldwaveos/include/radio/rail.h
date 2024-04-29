//
// Created by Sebastian Floss on 27.05.2023.
// Copyright (c) 2019 ImagineOn GmbH. All rights reserved.
//

#ifndef _COLDWAVEOS_RAIL_H
#define _COLDWAVEOS_RAIL_H

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <driver.h>

struct rail_driver {
    struct driver drv;
};


#if __cplusplus
}
#endif

#endif //_COLDWAVEOS_RAIL_H
