/********************************************************************************
  * @file    cmsis_os2_ext.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   Extended Kernel functions
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _CMSIS_OS2_EXT_H_
#define _CMSIS_OS2_EXT_H_

#include <kernel/cmsis_os2.h>
#include <stdarg.h>

#ifdef  __cplusplus
extern "C"
{
#endif


#ifndef EXCLUDE_FROM_DOCS



/// \details Ticker ID identifies the timer.
typedef void *osTickerId_t;

typedef uint32_t osDebugFlags_t;

typedef void *osMessageBufferId_t;

/// Ticker callback function.
typedef void (*osTickerFunc_t) (void *argument);

void osHeapInitialize (void);

#endif



/** @addtogroup core_debug
 *  @{
 */
#define OS_DBGFLAG_HALT_ON_ERR    (0x00000001U) ///< stop the system on any fault or assertion
#define OS_DBGFLAG_MEMORY         (0x00000002U) ///< periodically print the available heap memory
#define OS_DBGFLAG_CPU_LOAD       (0x00000004U) ///< periodically print all tasks and their cpu usage
#define OS_DBGFLAG_WATCHDOG       (0x00000008U) ///< debug watchdog and halt on "bite"
/** @} */


/**
 * @addtogroup threading_thread
 * @{
 */

///
/// \brief suspend all other threads and kernel space interrupts.
/// \return set of flags that need to be stored and passed to \ref osExitCriticalSection upon ending the critial section.
uint32_t osEnterCriticalSection (void);

///
/// \brief exit the critical section and resume normal operations
/// \param flags as obtained from a previous call to \ref osEnterCriticalSection
void osExitCriticalSection (uint32_t flags);


void osSetThreadLocalStoragePointer(void * pointer);

void *osGetThreadLocalStoragePointer(void);


/**  @} */


/** @addtogroup core_sleep
 *  @{
 */

///
/// \brief invoked by the kernel, when it wants to enter sleep mode
/// \return 0 to inform the kernel that the application can't sleep at the moment, 1 to accept the request
typedef int  (*osIdleSleepEnterEvent_t) (void);

///
/// \brief invoked by the kernel, after it wakes up from sleep mode
/// \details under normal circumstances the applicaton does not need to do anything here.
typedef void (*osIdleSleepExitEvent_t) (void);

/** @} */





/// Reboot the system
void osReboot (void);

/**
 * @defgroup core_sleep Power Saving
 * @brief Power saving functions
 * @details The RTOS kernel determines periods where it is idle and does not need to run.
 * It then can put the MCU into a basic sleep state by pausing until an interrupt occurs.
 * If this behaviour is desired, the following function should be called in program initialization,
 * i.e. early on in main().
 * @{
 */

///
/// \brief register two callbacks the kernel will invoke whenever it wants to enter or exit sleep.
/// \param[in]     enter_sleep_event    callback that gets called by the scheduler before entering sleep.
/// \param[in]     enter_sleep_event    callback that gets called by the scheduler after waking up
void osKernelEnableIdleSleep (osIdleSleepEnterEvent_t enter_sleep_event, osIdleSleepExitEvent_t exit_sleep_event);

/// Replaces osKernelResume
void osKernelDisableIdleSleep (void);

/**  @} */


/**
 * @defgroup core_debug Debugging
 * @brief Debugging helper functions
 * @details The functions provided in the debugging group are meant to
 *  enable and disable certain debug outputs at runtime. As a developer you should make
 *  sure to disable any debug options in release code since it can significantly slow down
 *  execution and also increase RAM and Flash usage.
 * @{
 */

///
/// \brief Enable certain debugging behaviour
/// \param flags or'ed flags to enable
void osSetDebugFlags (osDebugFlags_t flags);

///
/// \brief Disable certain debugging behaviour
/// \param flags or'ed flags to disable
void osClearDebugFlags (osDebugFlags_t flags);

///
/// \brief Query the presence of a debugging probe (JTAG/SWD)
/// \details Be aware, that Cortex-M0 and -M0+ dont't support querying for a debugger.
///  Also, once a debugger was attached to the system, simply detaching it won't make the function return 0.
///  The state of "no debugger attached" will only be restored by a "cold" reboot / power-on reset.
/// \return 1 if a debugger is attached, 0 if not. In case of Cortex-M0/M0+ the function returns 1 for debug builds at any given time.
int osDebuggerAttached (void);

/**  @} */

/**
 * @defgroup core_datetime Date/Time
 * @brief System Date&Time functions
 * @details
 * These functions are used to set the date and time base of the operating system
 * based on UTC second-based unix timestamps.
 * The function \ref osSetSystemTime (uint32_t sec) will much likely never be called
 * by a user of the API, unless the system time needs to be entered via some kind of user-interface,
 * because the TCP/IP subsystem will try to retreive the time via NTP as soon as any network interface
 * goes up.
 * Please be aware, that currently there is no way to prevent the system from updateing the time
 * via NTP itself, whenever there is an internet connection available.
 * @{
 */

///
/// \brief Set the systems date/time base.
/// \details call this function only in situations where there are no network interfaces with
///  an IP-Connection to the internet are available, because the OS will utilize NTP to set its
///  timebase automatically when possible.
///  The function is only made avaiable in the public API for applications that don't have network interfaces
///  and need to set a time and date manually via some user interface.
/// \param sec the system timebase in seconds as Unix-Timestamp
void osSetSystemTime (uint32_t sec);


/**  @} */

/**
 * @defgroup core_dispatch Dispatch
 * @brief Asynchronous dispatch queue
 * @details The asynchronous dispatch queue provides a means of delegating a function call
 * from the current execution context into a "background" thread.
 * This is useful if the caller has a very limited amount of stack memory and can't tell
 * what the callee's stack requirements are.
 * Also, in situations where the calling function execution is time-critial whilst the callee
 * is not, by dispatching the call the caller can continue to execute in deterministic time.
 * @{
 */

/// \brief async dispatch queue function prototype
typedef void (*osDispatchQueueCbFunc_t) (void *);

///
/// \brief a utility function to execute a function asynchronously.
/// \details this functionality makes sense, when something needs to be dispatched from a
///  time-critical section of code, where the time it takes the callee to execute is non deterministic,
///  or not known to the caller.
///  The stack-memory available for the callee at execution time currently is fixed at 1024 bytes.
///  It will be made configurable via the SysConfig in a future release.
/// \param f the function to be dispatched asynchronously
/// \param params pointer to a user-defined set of parameters that are passed to f().
/// \return osOK if the call was dispatched sucessfully. osError if it failed, most likely due to low resources.
osStatus_t osDispatchAsync (osDispatchQueueCbFunc_t f, void *params);

/**  @} */

/**
 * @defgroup core_wdt Watchdog
 * @brief System watchdog functions
 * @details The system watchdog automatically checks for thread starvation or hung-up resources without
 * the need for the implementor to write any extra functionality. It defaults to a 10 second timeout in which
 * it gets implicitly fed by calling any function that waits on a thread for a
 * definite amount of time. Only in cases where indefinite waits (osWaitForever) are required the implementor
 * must explicitly call osWatchdogFeed() at least once every 10 seconds or suspend the watchdog beforehand.
 * @{
 */

///
/// \return staus code. osOK if the watchdog was successfully fed.
osStatus_t osWatchdogFeed (void);

/// Suspends the watchdog for the calling thread until it either gets resumed by a call to osWatchdogResumeForThread
/// or any call to any function that suspends the thread for a definite time.
/// Usually this function gets called before functions that wait indefinitely, e.g. reads from a UART.
/// \return staus code. osOK if the message was successfully suspended
osStatus_t osWatchdogSuspendForThread (void);

/// Resume the watchdog for a previously suspended thread.
/// \return staus code. osOK if the watchdog was successfully resumed.
osStatus_t osWatchdogResumeForThread (void);

/**  @} */



/**
 * @defgroup threading_msgbuf Message Buffer
 * @brief Lightweight inter-thread message passing
 * @details message buffers can be used to pass variably-sized messages between two threads.
 *  the important difference to Message-Queues (aside from the variable size of the messages) is
 *  that message buffers should never be used to pass information between more than two threads.
 *  There should be one producer and one consumer only.
 *
 *  ### Sysconfig #######
 *  First Header  | Second Header
 *  ------------- | -------------
 *  Content Cell  | Content Cell
 *  Content Cell  | Content Cell
 *
 *  @code
 *      Here is some example code on how to use them.
 *  @endcode
 * @{
 */

///
/// \brief creates a new message buffer
/// \param bufferSize maximum size of the total buffer (not just a single message) in bytes.
/// \return message buffer ID to be used with send and receive functions.
osMessageBufferId_t osMessageBufferNew (uint32_t bufferSize);

///
/// \brief send a message to the receiving end of the buffer
/// \param mb_id message buffer ID as obtained from osMessageBufferNew.
/// \param msg_ptr pointer to the message. if the message is dynamically allocated,
/// it is up to the recipient to free this memory. If the call fails the caller has to free the memory.
/// \param msg_len length of the message in bytes.
/// \param timeout timeout to wait for available buffer space ( in milliseconds )
/// \return staus code. osOK if the message was successfully sent.
osStatus_t osMessageBufferSend (osMessageBufferId_t mb_id, const void *msg_ptr, size_t msg_len, uint32_t timeout);

///
/// \brief wait for a message
/// \param mb_id message buffer ID as obtained from osMessageBufferNew.
/// \param msg_ptr pointer to the memory where the received message should be put.
/// \param msg_len size of the memory region that mst_ptr points to.
/// \param timeout timeout to wait for a message to arrive ( in milliseconds )
/// \return staus code. osOK if a message was retrieved
osStatus_t osMessageBufferReceive (osMessageBufferId_t mb_id, void *msg_ptr, size_t *msg_len, uint32_t timeout);

///
/// \brief reset the message buffer. only possible if no resource is blocking on the buffer
/// \param mb_id message buffer ID as obtained from osMessageBufferNew.
/// \return staus code. osOK if reset was successful
osStatus_t osMessageBufferReset (osMessageBufferId_t mb_id);

/** @} */


/**
 * @addtogroup core_timer
 * @details The microsecond "Ticker" is a hardware-dependent feature and relies on an implementation
 * in the specific HAL. It utilizes (and therefore occupies) timer hardware, offering precise periodic
 * function calls, for example in time-critical sensor readouts.
 * If a HAL library does not implement it, all functions will return "0" instead of a handle.
 * @{
 */

///
/// \brief creates a new ticker instance.
/// \param func callback function to be executed every interval
/// \param arg pointer to be passed to the callback function upon invocation
/// \param microseconds interval the callback should be invoked at.
/// \param stack_size the stack size to allocate for the callback function.
/// \return Id of the ticker instance created.
osTickerId_t osTickerNew (osTickerFunc_t func, void *arg, uint32_t microseconds, uint32_t stack_size);

///
/// \brief destroy a ticker instance.
/// \param ticker Id of the ticker instance obtained from a previous call to osTickerNew
/// \return status. osOK if the instance was deleted successfully.
osStatus_t osTickerDelete (osTickerId_t ticker);

///
/// \brief start executing a ticker instance.
/// \param ticker Id of the ticker instance obtained from a previous call to osTickerNew
/// \return status. osOK if the instance was started successfully.
osStatus_t osTickerStart (osTickerId_t ticker);

///
/// \brief stop executing a ticker instance.
/// \param ticker Id of the ticker instance obtained from a previous call to osTickerNew
/// \return status. osOK if the instance was stopped successfully.
osStatus_t osTickerStop (osTickerId_t ticker);

/**  @} */


#ifdef  __cplusplus
}
#endif

#endif //_CMSIS_OS2_EXT_H_

