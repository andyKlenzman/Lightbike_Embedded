/*******************************************************************************
 * @file       kernel.h
 * @brief
 * @copyright  Copyright (c) 2018-2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef _COLDWAVEOS_KERNEL_H
#define _COLDWAVEOS_KERNEL_H


typedef void (*pFunc)(void);

typedef struct platform_init_t {
    pFunc func;
    const char* name;
} platform_init_t  ;

#define cw_reboot_safe __attribute__((section(".cw_reboot_safe")))

#define CW_PLATFORM_INIT_FUNC(x, n) \
                  void x (void); \
                  platform_init_t cw_platform_init_ ## x  __attribute__((section(".cw_platform_init"))) = \
                    { .func = x, .name = #n }; \
                  void x (void)  \

#include <kernel/cmsis_os2.h>
#include <kernel/cmsis_os2_ext.h>

#endif //_COLDWAVEOS_KERNEL_H
