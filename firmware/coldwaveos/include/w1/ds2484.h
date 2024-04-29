
#ifndef CXOS_DS2484_H
#define CXOS_DS2484_H

#include <stdbool.h>
#include <kernel.h>

#if defined (__cplusplus)
extern "C" {
#endif

void ds2484_init(int i2c_dev, bool od);

void ds2484_reset();

#if defined (__cplusplus)
}
#endif

#endif // CXOS_DS2484_H