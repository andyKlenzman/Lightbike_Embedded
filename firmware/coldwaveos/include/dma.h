/********************************************************************************
  * @file    dma.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   SPI HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_DMA_H
#define _COLDWAVEOS_DMA_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif


typedef void (*dma_cb)(uint8_t, void *);



/**
 * @defgroup drv_dma DMA
 * \ingroup drv
 * @brief DMA Driver
 * @details Functions for interacting with peripheral devices via DMA
 * @{
 */
#ifndef EXCLUDE_FROM_DOCS
struct dma_driver {
    struct driver drv;
    int (*dma_acquire_channel)(struct device *, dma_cb, void *);
    int (*dma_release_channel)(struct device *, unsigned int);
};
#endif

///
/// \param hDev
/// \param cb
/// \return
CW_DRIVER_FUNC
dma_acquire_channel(int hDev, dma_cb cb, void *p) CW_DRIVER_CALL(dma, acquire_channel, hDev, cb, p);

///
/// \param hDev
/// \param channel
/// \return
CW_DRIVER_FUNC
dma_release_channel(int hDev, unsigned int channel) CW_DRIVER_CALL(dma, release_channel, hDev, channel);


/**  @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_DMA_H
