/*******************************************************************************
 * @file       uart.h
 * @brief
 * @copyright  Copyright (c) 2020-2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef _COLDWAVEOS_UART_H
#define _COLDWAVEOS_UART_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
* @defgroup drv_serial
* \ingroup drv
* @brief Abstraction API for serial peripherals.
* @details
* @{
*/

/**
 * @defgroup drv_serial_uart UART
 * \ingroup drv_serial
 * @brief UART Serial API
 * @details API functions for bidirectional Asynchronous serial communications
 *
 * Sysprops
 * --------
 *
 * sysprop   | description
 * ----------|------------------------------------
 * gpio_tx  | RX GPIO
 * gpio_rx  | TX GPIO
 * gpio_rts | RTS GPIO
 * gpio_cts | CTS GPIO
 *
 * @{
 */

#define   UART_IOCTL_SEND_BREAK              (0x0F01)

#ifndef EXCLUDE_FROM_DOCS
struct uart_driver {
    struct driver drv;
    // uart_register_event
    // uart_get_capabilities
	/*
uint32_t	dtr: 1	DTR Line: 0=not available, 1=available.
uint32_t	dcd: 1	DCD Line: 0=not available, 1=available.
uint32_t	event_dcd: 1	Signal DCD change event: ARM_USART_EVENT_DCD.
uint32_t	event_ri: 1	Signal RI change event: ARM_USART_EVENT_RI.
     */
	// uart_write_abort
	// uart_read_abort
	// uart_modem_set_dtr (bool)
	// uart_modem_get_dcd (void)

	int (*uart_config) (struct device *, uint32_t, uint8_t, uint8_t, uint8_t);
	int (*uart_write) (struct device *, char *, size_t, uint32_t);
	int (*uart_read) (struct device *, char *, size_t, uint32_t);
    };
#endif

///
/// \brief configure baudrate and hardware protocol for the UART.
/// \param hDev device handle as returned by \ref open(const char*)
/// \param baudrate desired baudrate. the peripheral driver documentation should be consulted for supported baudrates
/// \param bits databits in the UART protocol, ususally 8
/// \param stop number of stop-bits in the UART protocol. since 1.5 stop bits is no longer used in modern devices, it is not supported here.
/// \param parity 0 for no parity, 1 for odd and 2 for even parity
CW_DRIVER_FUNC uart_config (int hDev, uint32_t baudrate, uint8_t bits, uint8_t stop, uint8_t parity)
CW_DRIVER_CALL(uart, config, hDev, baudrate, bits, stop, parity);

///
/// \brief write a character sequence over the UART
/// \details the function acts as blocking during a write to simplify the application architecture.
/// \param hDev device handle as returned by \ref open(const char*)
/// \param buf pointer to a char buffer. the buffer is not copied in the function, so it needs to be valid until the function returns.
/// \param buflen length of the passed buffer.
/// \param timeout_ms timeout in milliseconds to wait for the buffer to be transmitted
/// \return 0 on successful write.
CW_DRIVER_FUNC uart_write (int hDev, char *buf, size_t buflen, uint32_t timeout_ms)
CW_DRIVER_CALL(uart, write, hDev, buf, buflen, timeout_ms);
///
/// \brief read a character sequence from the UART
/// \details the function will return after
///  * the timeout has passed.
///  * n > 0 characters have been received, followed by a timeout of at least 2 character-times.
///  * the passed buffer is full.
///
/// \param hDev device handle as returned by \ref open(const char*)
/// \param buf buffer to hold the received characters.
/// \param buflen length of the provided buffer
/// \param timeout_ms timeout in milliseconds to wait for buflen characters to be returned
/// \return number of characters read
CW_DRIVER_FUNC uart_read (int hDev, char *buf, size_t buflen, uint32_t timeout_ms)
CW_DRIVER_CALL(uart, read, hDev, buf, buflen, timeout_ms);


/**  @} */
/**  @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_UART_H
