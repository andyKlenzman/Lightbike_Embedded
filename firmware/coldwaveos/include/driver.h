/*******************************************************************************
 * @file       driver.h
 * @brief
 * @copyright  Copyright (c) 2020-2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/


#ifndef _COLDWAVEOS_DRIVER_H
#define _COLDWAVEOS_DRIVER_H

#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sysconfig.h>
#include "macro.h"

#define     DRIVER_MAX_FD           32
#define     DRIVER_NAME_MAX_LEN     32
#define     DEVICE_NAME_MAX_LEN     8

#if defined (__cplusplus)
extern "C" {
#endif

/**
* @addtogroup core
* @{
*/

/**
 * @defgroup core_driver Peripheral Drivers
 * @brief Peripheral Driver API
 * @details The following functions are needed to access peripheral icm20649
 *  before using them. The functions \ref open(const char *) and \ref close(int) are
 *  used to manage power saving and concurrency. Some devices like UARTs or ADC/DACs can only be opened once
 *  and need to be closed first before the device can be accessed from another thread while
 *  others (I2C e.g.) can be opened multiple times.
 *  The API also makes sure that a device that currently isn't open anywhere can be put to sleep in order
 *  to save power.
 * @{
 */

/**/


#ifndef EXCLUDE_FROM_DOCS

typedef int cw_driver_return_t;

#define CW_SYSPROP_MAKE_NAME(drv, prop) sysconf_param_ ##drv ## _ ## prop

#define CW_HAS_SYSPROP(dev, prop) sysconf_has_prop (dev, sysconf_param_ ## prop)
#define CW_HAS_CUSTOM_SYSPROP(drv, dev, prop) sysconf_has_prop (dev, CW_SYSPROP_MAKE_NAME(drv,prop))

#define CW_GET_SYSPROP_INT(dev, prop) sysconf_get_int_prop (dev, sysconf_param_ ## prop)
#define CW_GET_SYSPROP_DEVICE(dev, prop) sysconf_get_device_prop (dev, sysconf_param_ ## prop)
#define CW_GET_SYSPROP_STRING(dev, prop) sysconf_get_string_prop (dev, sysconf_param_ ## prop)

#define CW_GET_CUSTOM_SYSPROP_INT(drv, dev, prop) sysconf_get_int_prop (dev, CW_SYSPROP_MAKE_NAME(drv,prop))
#define CW_GET_CUSTOM_SYSPROP_DEVICE(drv, dev, prop) sysconf_get_device_prop (dev, CW_SYSPROP_MAKE_NAME(drv,prop))
#define CW_GET_CUSTOM_SYSPROP_STRING(drv, dev, prop) sysconf_get_string_prop (dev, CW_SYSPROP_MAKE_NAME(drv,prop))

#define CW_GPIO_CONSUME(dev, gpio)

#define cw_initLevelPreKernel       0
#define cw_initLevelPostKernel0     1
#define cw_initLevelPostKernel1     2

#define cw_initInBootloaderAndApp    1
#define cw_initInBootloaderOnly      2

#define CW_DRIVER_STRUCT __attribute__((unused))

#define CW_DRIVER_FUNC static inline cw_driver_return_t

#define CW_DRIVER_PLATFORM_DATA_T(drv)  drv ## _platform_data_t

#define CW_DRIVER_SYSPROP_DECL(drv, prop) \
    extern const uint32_t CW_SYSPROP_MAKE_NAME(drv,prop) \
    __attribute__ ((weak, alias ("cw_sysprop_empty"), section((".cw_sysprops"))));

#define CW_DRIVER_PLATFORM_DATA(drv, dev) ((CW_DRIVER_PLATFORM_DATA_T(drv)*)((struct device*)(dev))->platform_data)

#define CW_DRIVER_PLATFORM_DATA_CALLOC(drv, dev) \
         ((struct device*)(dev))->platform_data = malloc(sizeof(CW_DRIVER_PLATFORM_DATA_T(drv))); \
         memset (((struct device*)(dev))->platform_data , 0, sizeof(CW_DRIVER_PLATFORM_DATA_T(drv)))

#define CW_DRIVER_CALL(cls, name, hnd, ...) \
  { struct device *dev = device_for_handle (hnd); \
  if (dev && ((struct cls ## _driver *) dev->drv)->cls ## _ ## name) \
    { return ((struct cls ## _driver *) dev->drv)->cls ## _ ## name (dev, __VA_ARGS__); } \
  return -ENODEV; }

#define CW_DRIVER_CALL_VOID(cls, name, hnd) \
  { struct device *dev = device_for_handle (hnd); \
  if (dev && ((struct cls ## _driver *) dev->drv)->cls ## _ ## name) \
    { return ((struct cls ## _driver *) dev->drv)->cls ## _ ## name (dev); } \
  return -ENODEV; }

#define CW_DRIVER_SYSTEM_DEVICE_REGISTER(drv_id, drv_name, dev_id, io_base, ...) \
               static uint32_t CAT(__driver_reg_struct_,drv_id)                                 \
               __attribute__ ((section(".cw_driver_init"), used)) = (uint32_t)&CAT(drv_id,_drv);   \
               const struct cw_sysconf_param_entry __attribute__((section(".cw_sysconfig_params")))   \
                            pe_ ## dev_id[PP_NARG(__VA_ARGS__) / 4 + 3] =  {                    \
              sysconf_set_str_param(driver_name, drv_name),                                     \
              sysconf_set_str_param(device_name, #dev_id),                                      \
              sysconf_set_int_param(peripheral_base, io_base),                                  \
                __VA_ARGS__                                                                     \
              };                                                                                \
              const struct cw_sysconf_device_entry  __attribute__ ((section(".cw_sysconfig"), used))  \
              CAT(dev_id, _systbl) = {                                                          \
                  .params = pe_ ## dev_id,                                                      \
                  .num_params = PP_NARG(__VA_ARGS__) / 4 + 3                                    \
                  };

#define CW_DRIVER_BL_SYSTEM_DEVICE_REGISTER(drv_id, drv_name, dev_id, io_base, ...) \
               static uint32_t CAT(__driver_reg_struct_,drv_id)                                 \
               __attribute__ ((section(".cw_bl_driver_init"), used)) = (uint32_t)&CAT(drv_id,_drv);   \
               const struct cw_sysconf_param_entry __attribute__((section(".cw_sysconfig_params")))   \
                            pe_ ## dev_id[PP_NARG(__VA_ARGS__) / 4 + 4] =  {                    \
              sysconf_set_str_param(driver_name, drv_name),                                     \
              sysconf_set_str_param(device_name, #dev_id),                                      \
              sysconf_set_int_param(peripheral_base, io_base),                                  \
              sysconf_set_int_param(bootloader, cw_initInBootloaderAndApp),                     \
               __VA_ARGS__                                                                      \
              };                                                                                \
              const struct cw_sysconf_device_entry  __attribute__ ((section(".cw_bl_sysconfig"), used))  \
              CAT(dev_id, _systbl) = {                                                          \
                  .params = pe_ ## dev_id,                                                      \
                  .num_params = PP_NARG(__VA_ARGS__) / 4 + 4                                    \
                  };

#define CW_DRIVER_BL_ONLY_DEVICE_REGISTER(drv_id, drv_name, dev_id, io_base, ...) \
               static uint32_t CAT(__driver_reg_struct_,drv_id)                                 \
               __attribute__ ((section(".cw_bl_driver_init"), used)) = (uint32_t)&CAT(drv_id,_drv);   \
               const struct cw_sysconf_param_entry __attribute__((section(".cw_sysconfig_params")))   \
                            pe_ ## dev_id[PP_NARG(__VA_ARGS__) / 4 + 4] =  {                    \
              sysconf_set_str_param(driver_name, drv_name),                                     \
              sysconf_set_str_param(device_name, #dev_id),                                      \
              sysconf_set_int_param(peripheral_base, io_base),                                  \
              sysconf_set_int_param(bootloader, cw_initInBootloaderOnly),                       \
               __VA_ARGS__                                                                      \
              };                                                                                \
              const struct cw_sysconf_device_entry  __attribute__ ((section(".cw_bl_sysconfig"), used))  \
              CAT(dev_id, _systbl) = {                                                          \
                  .params = pe_ ## dev_id,                                                      \
                  .num_params = PP_NARG(__VA_ARGS__) / 4 + 4                                    \
                  };

struct device;

extern uint32_t cw_driver_init_start, cw_driver_init_end;

struct driver {

    char name[DRIVER_NAME_MAX_LEN];

    int initLevel;

    int (*initialize) (struct device *dev);

    int (*suspend) (struct device *dev);

    int (*resume) (struct device *dev);

    int (*uninitialize) (struct device *dev);

    ssize_t (*write) (struct device *dev, const void *dataptr, size_t size);

    ssize_t (*read) (struct device *dev, void *mem, size_t len);

    int (*ioctl)(struct device *dev, long cmd, void *argp);

};

struct device
	{

	uint32_t id;
	char name[DEVICE_NAME_MAX_LEN];
	void *platform_data;
	struct driver *drv;
	struct device *parent;
	uint32_t io_base;

	};

#endif

///
/// \brief opens a device for use by the caller and returns a handle to the device
/// \details this function might return -1 in two situations: a) the device does not exist
/// b) the device can only be opened once and has already be opened.
/// \param device_name the string that has been assigned to the device in the SysConf
/// \return the handle for referencing the device in calls to driver functions or -1 on failure.
int
open(const char *device_name);

///
/// \brief closes a device and makes it available for other threads and power sleep.
/// \param hdev the handle returned from a previous call to \ref open(const char*).
/// \return 0 on successful close, -1 if either the handle doesn't exist or the device is not open.
int
close (int hdev);

#ifndef EXCLUDE_FROM_DOCS

struct device *
create_device(struct driver *drv);

struct device *
device_for_handle(int hDev);

struct driver *
driver_for_handle(int hDev);

#endif

/**  @} */
/**  @} */


#if defined (__cplusplus)
}
#endif
#endif //_COLDWAVEOS_DRIVER_H
