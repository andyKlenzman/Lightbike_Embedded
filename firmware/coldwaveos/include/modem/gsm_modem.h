/*******************************************************************************
 * @file     gsm_modem.h
 * @brief
 * @copyright  Copyright (c) 2023. ImagineOn GmbH. www.imagineon.de
 * All rights reserved.
 ******************************************************************************/
/*******************************************************************************
For each file, could it be described what is optional. What is necessary overhead. What if I 
have a project that does not use LTE and tight mem. constraints?


In what context will these modum macros be used? Why does it not explain their purpose in the file, 
it is not evident to a beginner what this is or if it applies to their project. 

I did not know what a modem was. 

 ******************************************************************************/

/*******************************************************************************
Modem - a modulator-demodulator, allows for communication between devices over 
analog or digital means. It converts analog to digital and vis versa (super cool)

GSM_Modem - global systems for mobile communication. Uses a sim card to authenticate users and used for 
2g and 3g comms.

What is the capacity of 2g and 3g comms? 


Strange to me that macros dont need scope resolution, seems like they could easily 
conflict and cause same problems as function naming conflicts, which do require it

My understanding was that a header file always comes with cpp file. For a beginner, these advanced 
uses of cpp should be explained. What is an example of excellent inline comments and documentation on an 
open source project



 ******************************************************************************/



#ifndef _GSM_MODEM_H_
#define _GSM_MODEM_H_

#include <kernel.h>
#include <driver.h>

/*******************************************************************************
What are all of these addresses for. Why are there so many?


These are LTE (long term evolution) frequency bands. Each hex represents the digital representation
of that band??

LTE is a broadband communication technology for cellular connection
Broadband is always on and does not need to establish a connection each time
How does that work, why does bluetooth need a connection
Broadband communications happens over a wide range of frequencies
and can support transmission of multiple data channels.

Do different bandwidths(Hz) have different bit rate(bps, so kbps,mbps, or gbps)

Cellular comes from the structure of the communication network
Comms towers  are organized into geographic areas called cells
 ******************************************************************************/
#define LTE_B1			    0x1
#define LTE_B2			    0x2
#define LTE_B3			    0x4
#define LTE_B4			    0x8
#define LTE_B5			    0x10
#define LTE_B8			    0x80
#define LTE_B12			    0x800
#define LTE_B13			    0x1000
#define LTE_B14 		    0x2000
#define LTE_B18 		    0x20000
#define LTE_B19 		    0x40000
#define LTE_B20 		    0x80000
#define LTE_B25 		    0x1000000
#define LTE_B26 		    0x2000000
#define LTE_B27 		    0x4000000
#define LTE_B28 		    0x8000000
#define LTE_B31 		    0x40000000
#define LTE_ALL             0x4001C2000000004E0E189F

#define GSM_MODEM_IOCTL_START_RF_TEST       (0x0f01)
#define GSM_MODEM_IOCTL_STOP_RF_TEST        (0x0f02)


#if defined (__cplusplus)
extern "C" {
#endif


/**
 * @defgroup drv_networking_gsmmodem GSM modem
 * \ingroup drv_networking
 * @brief GSM modem driver
 * @details
 *
 * To use the GSM modem driver you need to retrieve a handle to the GSM modem device
 * by a call to \ref open(const char*) supplying the device name as specified in the
 * sysconf.
 * Afterwards call \ref gsm_modem_init (int, gsm_modem_init_t *) to have the modem start
 * searching for a mobile network operator and attach to the PS domain.
 * @code
 * int h_modem = open("gsm0");
 * gsm_modem_init_t modem_init = GSM_MODEM_INIT_DEFAULT;
 * memmove (modem_init.apn, PDP_CONTEXT_NAME, strlen (PDP_CONTEXT_NAME));
 * gsm_modem_init(h_modem, &modem_init);
 * @endcode
 *
 * Sysprops
 * --------
 *
 * The Modem needs a parent UART device with RTS&CTS
 *
 * sysprop   | description
 * ----------|------------------------------------
 * gpio_ri   | Ring-Indicator GPIO
 * gpio_dcd  | Data Carrier Detect (DCD) GPIO
 * gpio_dtr  | Data Terminal Ready (DTR) GPIO
 *
 * @code
 *
sysconf_create_device("silabs-gecko-euart", eusart1, 0x500A0000UL ,
                      sysconf_set_int_param (gpio_rx, 206),
                      sysconf_set_int_param (gpio_tx, 205),
                      sysconf_set_int_param (gpio_rts, 204),
                      sysconf_set_int_param (gpio_cts, 203))

sysconf_create_device("quectel-bg77", modem0, 0x0,
                      sysconf_set_parent_dev (eusart1),
                      sysconf_set_int_param (gpio_dcd, 201),
                      sysconf_set_int_param (gpio_dtr, 207),
                      sysconf_set_int_param (gpio_ri, 200),
                      sysconf_set_int_param (quectel_bg77_pwrkey_gpio, 208),
                      sysconf_set_int_param (gpio_status, 202))
 * @endcode
 *
 * @{
 */

/// @brief Default init parameters for GSM modem, setting it to LTE CAT-M1 on Band 8
#define GSM_MODEM_INIT_DEFAULT = {  \
  .bands = LTE_B8,                    \
  .power_on_reset = 1,                \
  .use_peer_dns = 1,                  \
  .edrx = 0,                          \
  .psm = 0,                           \
  .oper = {0},                        \
  .apn_auth_type = atCHAP,            \
  .tech = gtEMMC,                     \
  .baudrate = 921600,                 \
  .apn = {0},                         \
  .scanseq[0] = 0,                    \
  .no_auto_attach = 0,                \
  .apn_user = {0),                    \
  .apn_pwd = {0}                      \
  }

/// @brief Modem Radio Access Technology (RAT)
typedef enum
{
	gtNBIOT,        ///< LTE CAT-NB1 / CAT-NB2
	gtEMMC,         ///< LTE CAT-M1
	gtEMMC_NBIOT,   ///< CAT-M1 and LTE CAT-NB1 / CAT-NB2 and
    gtNBIOT_EMMC,   ///< LTE CAT-NB1 / CAT-NB2 and CAT-M1
    gtGSM,          ///< GPRS / UMTS
    } gsm_technology_t;

/// @brief APN Authentication Type for PPP
typedef enum
    {
    atNONE,          ///< No Authentication required
    atPAP,           ///< PAP Authentication only
    atCHAP,          ///< CHAP Authentication only
    atPAPCHAP        ///< Both PAP and CHAP are supported
    } apn_auth_t;

/**
 *  @brief GSM modem initialization parameters.
 */
typedef struct gsm_modem_init_t {

    uint32_t bands;           ///< Bitfield containing the or'ed frequency bands the modem should search
    char oper[8];             ///< numeric operator code (3-digit MCC, 2-digit MNC, <a href="https://en.wikipedia.org/wiki/Mobile_Network_Codes_in_ITU_region_2xx_(Europe)#Germany_–_DE">List of MNC for the EU</a>). If left empty (0) the modem selects from the operators stored on the SIM card.
    gsm_technology_t tech;    ///< Radio access technology (RAT) to use
    uint8_t power_on_reset;      ///< Force a modem power on reset
    uint8_t edrx;             ///< Extended discontinuous reception settings
    uint8_t psm;              ///< Power saving mode settings
    char apn[100];            ///< APN to use
    apn_auth_t apn_auth_type; ///< PPP authentication type for the APN
    char apn_user[128];       ///< PPP APN username
    char apn_pwd[512];        ///< PPP APN password
    char scanseq[10];         ///< the order in which different RATs shall be preferably used
    uint8_t use_peer_dns;     ///< 1= use dns servers from PPP negotiation, 0= use custom DNS server
    uint8_t no_auto_attach;   ///< 0=  attach to the network on init, 1= attach only when calling connect.
    uint32_t baudrate;        ///< Baudrate for the modem. If set to 0 default is 115200;
    } gsm_modem_init_t;

/**
 * @brief Signal Strength Information
 */
typedef struct gsm_signal_info_t
    {
    char conn_type[20]; ///< RAT identifier as string. "NB-IoT", "eMMC", "GSM"
    int RSSI; ///< 'received signal strength indicator' see vendor manual for interpretation. recommended values: -65 dBm = excellent ... <= -110 dBM = no signal
    int RSRP; ///< optional/vendor-dependent: 'reference signal received power' in dBm
    int SINR; ///< optional/vendor-dependent: 'signal to interference plus noise ratio' Values are in 1/5th of a dB. Range: 0–250 which translates to -20 dB to +30 dB
    int RSRQ; ///< optional/vendor-dependent: 'reference signal received quality' in dBm
    } gsm_signal_info_t;



struct gsm_modem_driver {

    struct driver drv;
    int  (*gsm_modem_init)           ( struct device*, gsm_modem_init_t*  );
    int  (*gsm_modem_reset)          ( struct device* );
    int  (*gsm_modem_rf_on)          ( struct device* );
    int (*gsm_modem_rf_off) (struct device *);
    int (*gsm_modem_sleep) (struct device *);
    int (*gsm_modem_wakeup) (struct device *);
    int (*gsm_modem_connect) (struct device *, int);
    int (*gsm_modem_disconnect) (struct device *);
    int (*gsm_modem_connected) (struct device *);
    int (*gsm_modem_imei) (struct device *, char *);
    int (*gsm_modem_imsi) (struct device *, char *);
    int (*gsm_modem_signal_quality) (struct device *, gsm_signal_info_t *);

    };



/**
 * @brief Initialize the modem
 * @details  This function has to be called before any other gsm_modem_xx functions.
 * If not, behaviour will be unpredictable.
 * @param hdev device handle as returned by \ref open(const char*)
 * @param init structure holding the device configuration parameters
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_init (int hdev, gsm_modem_init_t *init);

/**
 * @brief resets the modem.
 * @details if the modem was connected to the PS domain before calling reset, it will
 * be disconnected and \ref gsm_modem_connect(int hdev, int wait) has to be called again.
 * @param hdev device handle as returned by \ref open(const char*)
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_reset (int hdev);

/**
 * @brief activates the radio interface of the modem
 * @param hdev device handle as returned by \ref open(const char*)
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_rf_on (int hdev);

/**
 * @brief turns the radio interface of the modem off
 * @param hdev device handle as returned by \ref open(const char*)
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_rf_off (int hdev);

/**
 * @brief puts the modem in power save mode immediately
 * @details the modem will not automatically wake up when a previously set PSM timer runs out.
 * It has to be explicitly woken up by \ref gsm_modem_wakeup (int hdev).
 * Any open connection to the PS-Domain will be closed and has to be re-established after waking
 * the modem up again. An attachment to the network will be kept for the time, the MNO confirmed after
 * requesting a PSM setting.
 * @param hdev device handle as returned by \ref open(const char*)
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_sleep (int hdev);

/**
 * @brief wakes the modem from PSM sleep immediately
 * @param hdev device handle as returned by \ref open(const char*)
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_wakeup (int hdev);

/**
 * @brief The function creates a PDP context and connects the modem to the APN.
 * @param hdev device handle as returned by \ref open(const char*)
 * @param wait if 0 is passed, the function returns immediately, trying to connect in the background.
 * @details the connection status can be queried by a call to \ref gsm_modem_connected(int) . if 1 is passed, the function blocks
 * until the connection either succeeds or fails.
 * @return if the modem has succesfully connected, the function returns 0, otherwise -1. In case `wait` is set to zero,
 * the function will only check for successful attachment to the PS-domain but not wait for the PDP-context establishment
 * or the PPP connection setup and return with 0 (=OK) immediately.
 */
int
gsm_modem_connect (int hdev, int wait);

/**
 * @brief The function closes the PDP context and disconnects the modem from the APN.
 * @param hdev device handle as returned by \ref open(const char*)
 * @return if the modem is disconnected succesfully the function returns 1, otherwise 0.
 */
int
gsm_modem_disconnect (int hdev);

/**
 * @brief checks the modem's current connection status in the PS domain
 * @param hdev device handle as returned by \ref open(const char*)
 * @return if the modem is connected to the PS domain the function returns 1, otherwise 0.
 */
int
gsm_modem_connected (int hdev);

/**
 *
 * @param hdev device handle as returned by \ref open(const char*)
 * @param imei reference to a character buffer of 15bytes, allocated by the called. The buffer will be filled with
 * the device identifier ("IMEI") upon success.
 * @return execution status 0 = Succeeded, any other value = Error
 */
int
gsm_modem_imei (int hdev, char* imei);

/**
 *
 * @param hdev device handle as returned by \ref open(const char*) device handle as returned by \ref open(char*)
 * @param imsi reference to a character buffer of 15bytes, allocated by the called. The buffer will be filled with
 * the subscriber identity ("IMSI") upon success.
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_imsi (int hdev, char* imsi);

/**
 *
 * @param hdev device handle as returned by \ref open(const char*)
 * @param info
 * @return 0 on success, -1 on failure
 */
int
gsm_modem_signal_quality (int hdev, gsm_signal_info_t* info);


/**  @} */



#if defined (__cplusplus)
}
#endif

#endif //_GSM_MODEM_H_


