/********************************************************************************
  * @file    eth.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   Ethernet HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_ETH_H_
#define _COLDWAVEOS_ETH_H_

#include <driver.h>
#include <inttypes.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @defgroup drv_networking_eth Ethernet
 * \ingroup drv_networking
 * @brief Ethernet Interface API
 * @details The API is a simplified API that does not offer all Ethernet MAC related
 *  configuration options, because the operating system has mainly been designed for
 *  battery-operated devices with RF interfaces. Nevertheless, the API allows for
 *  full-fledged TCP/IP communication over Ethernet.
 *
 *  Sysprops
 * --------
 * The Ethernet driver does not have any sysprops defined.
 *
 * @{
 */


#define ETH_MAC_SPEED_10M         (0)
#define ETH_MAC_SPEED 100M       (1)
#define ETH_MAC_SPEED_1G         (2)
#define ETH_MAC_DUPLEX_HALF      (0)
#define ETH_MAC_DUPLEX_FULL      (4)
#define ETH_MAC_LOOPBACK         (8)
#define ETH_MAC_CS_OFFLOAD_RX   (16)
#define ETH_MAC_CS_OFFLOAD_TX   (32)
#define ETH_MAC_ADDR_BROADCAST  (64)
#define ETH_MAC_ADDR_MULTICAST  (128)
#define ARM_ETH_MAC_ADDRESS_ALL (ETH_MAC_ADDR_BROADCAST | ETH_MAC_ADDR_MULTICAST)

/**
  * Interface configuration struct
  */
struct ethif_config
	{
	uint8_t hwaddr[6];   /**< the MAC address of the interface */
	char hostname[32];   /**< the interfaces NETBIOS name  */
	};

/**
  * Hardware address struct
  */
typedef struct eth_hw_address_t
	{
	uint8_t octet[6];
	} eth_hw_address_t;

/**
  * driver struct
  */
#ifndef EXCLUDE_FROM_DOCS
struct ethif_driver
	{

	struct driver drv;
	int (*ethif_config) (struct device *, int);
	int (*ethif_set_hwaddr) (struct device *, eth_hw_address_t *);
    int (*ethif_hwaddr) (struct device *, eth_hw_address_t *);
    int (*ethif_up) (struct device *, struct ethif_config *config);
    int (*ethif_down) (struct device *);
    };
#endif

///
/// \brief configures the low level behaviour of the device
/// \param hDev device handle as returned by \ref open(const char*)
/// \param flags
/// \return
CW_DRIVER_FUNC ethif_config (int hDev, int flags)
CW_DRIVER_CALL(ethif, config, hDev, flags);

///
/// \brief sets the device's hardware addres ('MAC address')
/// \param hDev device handle as returned by \ref open(const char*)
/// \param hwaddr
/// \return
CW_DRIVER_FUNC ethif_set_hwaddr (int hDev, eth_hw_address_t *hwaddr)
CW_DRIVER_CALL(ethif, set_hwaddr, hDev, hwaddr);

///
/// \brief gets the device's assgined hardware addres ('MAC address')
/// \param hDev device handle as returned by \ref open(const char*)
/// \param hwaddr
/// \return
CW_DRIVER_FUNC ethif_hwaddr (int hDev, eth_hw_address_t *hwaddr)
CW_DRIVER_CALL(ethif, hwaddr, hDev, hwaddr);

///
/// \brief activates the interface for TCP/IP
/// \param hDev device handle as returned by \ref open(const char*)
/// \param config structure containing the MAC address and Hostname to be used by the interface.
/// \return 0 if the interface was brought up successfully.
CW_DRIVER_FUNC ethif_up (int hDev, struct ethif_config *config)
CW_DRIVER_CALL(ethif, up, hDev, config);

///
/// \brief takes the interface down and detaches it from TCP/IP
/// \param hDev device handle as returned by \ref open(const char*)
/// \return 0 if the interface was brought down successfully
CW_DRIVER_FUNC ethif_down (int hDev)
CW_DRIVER_CALL_VOID(ethif, down, hDev);

/**  @} */



#if defined (__cplusplus)
}
#endif

#endif //_ETH_H_
