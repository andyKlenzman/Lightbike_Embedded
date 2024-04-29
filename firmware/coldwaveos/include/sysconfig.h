#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

#include <macro.h>
#include <driver.h>
#include <inttypes.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

static const uint32_t cw_sysprop_empty __section(".cw_sysprops") = 0;

#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61, _62, _63, N, ...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define sysconf_param_peripheral_base     100U
#define sysconf_param_bootloader          101U
  
#define sysconf_param_buffer_size         110U
#define sysconf_param_buffer_size_rx      111U
#define sysconf_param_buffer_size_tx      112U

#define sysconf_param_channel             200U

#define sysconf_param_gpio_tx             300U
#define sysconf_param_gpio_rx             301U
#define sysconf_param_gpio_cts            302U
#define sysconf_param_gpio_rts            303U
#define sysconf_param_gpio_dtr            304U
#define sysconf_param_gpio_dcd            305U
#define sysconf_param_gpio_ri             306U
#define sysconf_param_gpio_reset          307U
#define sysconf_param_gpio_sda            308U
#define sysconf_param_gpio_scl            309U
#define sysconf_param_gpio_miso           310U
#define sysconf_param_gpio_mosi           311U
#define sysconf_param_gpio_cs             312U
#define sysconf_param_gpio_clk            313U
#define sysconf_param_gpio_nss            314U
#define sysconf_param_gpio_status         315U
#define sysconf_param_gpio                316U

#define sysconf_param_baudrate            400U
#define sysconf_param_frequency_hz        401U
#define sysconf_param_period_ms           402U
#define sysconf_param_clock               403U
#define sysconf_param_clocksrc            404U
#define sysconf_param_pll_config          405U
#define sysconf_param_clocksrc_freq       406U
#define sysconf_param_stopbits            407U

#define sysconf_param_sample_rate         500U
#define sysconf_param_sample_buffer_size  501U

#define sysconf_param_slave               600U
#define sysconf_param_polarity_invert     601U
#define sysconf_param_swap_tx_rx          602U
#define sysconf_param_bus_address         603U
#define sysconf_param_mode                604U
#define sysconf_param_lsb_first           605U
#define sysconf_param_transfer_word_size  606U
#define sysconf_param_duplex              607U
#define sysconf_param_acvtive_high_cs     608U

#define sysconf_app_version(APP_VERSION, APP_VERSION_STR) \
      __attribute__((section(".version_info"), used)) \
      const struct ota_firmware_version_info_s ota_firmware_info = { \
          .firmware_info_version = APP_VERSION, \
          .firmware_version = APP_VERSION_STR \
      };

#define sysconf_init_stack(stack_size) \
            __attribute__((section(".cw_init_stack_size"))) \
            volatile uint32_t cw_init_stack_size = stack_size;


#define sysconf_use_driver(drv_id) \
                extern uint32_t CAT(drv_id,_drv); \
                static uint32_t CAT(__driver_reg_struct_,drv_id) \
                __attribute__ ((section(".cw_driver_init"), used)) = (uint32_t)&CAT(drv_id,_drv); \

#define sysconf_declare_param(param)    \
              const uint32_t  __attribute__  ((section(".cw_sysprops"))) sysconf_param_ ## param = (SYSPROP_USER_MIN_ID + __COUNTER__);



#define sysconf_create_device(drv_name, dev_id, io_base, ...)                                   \
              const struct cw_sysconf_param_entry __attribute__((section(".cw_sysconfig_params")))    \
                            pe_ ## dev_id[PP_NARG(__VA_ARGS__) / 4 + 3] =  {                    \
              sysconf_set_str_param(driver_name, drv_name),                                     \
              sysconf_set_str_param(device_name, #dev_id),                                      \
              sysconf_set_int_param(peripheral_base, io_base),                                  \
              __VA_ARGS__                                                                       \
              };                                                                                \
              const struct cw_sysconf_device_entry  __attribute__ ((section(".cw_sysconfig"), used))  \
              CAT(dev_id, _systbl) = {                                                          \
                  .params = pe_ ## dev_id,                                                      \
                  .num_params = PP_NARG(__VA_ARGS__) / 4 + 3                                    \
                  };

#define sysconf_set_str_param(id, val)   \
{                                       \
.param_desc._type = __SYSCONF_PT_STR,   \
.param_desc._id = sysconf_param_ ## id, \
.param_desc._len = sizeof(val)/sizeof(val[0]),  \
.param_val.str = val                    \
}

#define sysconf_set_int_param(id, val)  \
{                                       \
.param_desc._type = __SYSCONF_PT_INT,   \
.param_desc._id = sysconf_param_ ## id, \
.param_desc._len = 4,                   \
.param_val.i = val                      \
}

#define sysconf_set_dev_param(id, val)  \
{                                       \
.param_desc._type = __SYSCONF_PT_DEV,   \
.param_desc._id = sysconf_param_ ## id, \
.param_desc._len = 4,                   \
.param_val.ptr =  (void *) pe_ ## val   \
}

#define sysconf_set_parent_dev(val)     \
{                                       \
.param_desc._type = __SYSCONF_PT_DEV,   \
.param_desc._id =  sysconf_param_device_parent,  \
.param_desc._len = 4,                   \
.param_val.ptr =  (void *) pe_ ## val   \
}

struct cw_sysconf_param_desc {
    uint8_t _res: 1;
    uint8_t _type: 3;
    uint16_t _len: 11;
    uint16_t _id;
} __attribute__ (( packed ));

union cw_sysconf_param_val {
    uint32_t i;
    void *ptr;
    const char *str;
};

struct cw_sysconf_param_entry {
    struct cw_sysconf_param_desc param_desc;
    union cw_sysconf_param_val param_val;
};

struct cw_sysconf_device_entry {
    uint32_t num_params;
    const struct cw_sysconf_param_entry *params;
};

#define sysconf_param_driver_name    1
#define sysconf_param_device_name    2
#define sysconf_param_device_parent  3

#define SYSPROP_USER_MIN_ID      1000U

#define __SYSCONF_PT_PTR    (0x4)
#define __SYSCONF_PT_INT    (0x3)
#define __SYSCONF_PT_STR    (0x2)
#define __SYSCONF_PT_DEV    (0x1)

struct device;

uint32_t
sysconf_has_prop( struct device* dev, uint16_t property );

uint32_t
sysconf_get_int_prop( struct device* dev, uint16_t property );

struct device*
sysconf_get_device_prop( struct device* dev, uint16_t property );

const char*
sysconf_get_string_prop( struct device* dev, uint16_t property );


#ifdef __cplusplus
}
#endif

#endif