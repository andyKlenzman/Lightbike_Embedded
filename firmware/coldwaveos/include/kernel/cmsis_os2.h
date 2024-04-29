/*
 * Copyright (c) 2013-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ----------------------------------------------------------------------
 *
 * $Date:        18. June 2018
 * $Revision:    V2.1.3
 *
 * Project:      CMSIS-RTOS2 API
 * Title:        cmsis_os2.h header file
 *
 * Version 2.1.3
 *    Additional functions allowed to be called from Interrupt Service Routines:
 *    - osThreadGetId
 * Version 2.1.2
 *    Additional functions allowed to be called from Interrupt Service Routines:
 *    - osKernelGetInfo, osKernelGetState
 * Version 2.1.1
 *    Additional functions allowed to be called from Interrupt Service Routines:
 *    - osKernelGetTickCount, osKernelGetTickFreq
 *    Changed Kernel Tick type to uint32_t:
 *    - updated: osKernelGetTickCount, osDelayUntil
 * Version 2.1.0
 *    Support for critical and uncritical sections (nesting safe):
 *    - updated: osKernelLock, osKernelUnlock
 *    - added: osKernelRestoreLock
 *    Updated Thread and Event Flags:
 *    - changed flags parameter and return type from int32_t to uint32_t
 * Version 2.0.0
 *    Initial Release
 *---------------------------------------------------------------------------*/
/********************************************************************************
  * @file    cmsis_os2.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   Kernel functions.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef CMSIS_OS2_H_
#define CMSIS_OS2_H_

#ifndef __NO_RETURN
#if   defined(__CC_ARM)
#define __NO_RETURN __declspec(noreturn)
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __NO_RETURN __attribute__((__noreturn__))
#elif defined(__GNUC__)
#define __NO_RETURN __attribute__((__noreturn__))
#elif defined(__ICCARM__)
#define __NO_RETURN __noreturn
#else
#define __NO_RETURN
#endif
#endif

#include <stdint.h>
#include <stddef.h>


#ifdef  __cplusplus
extern "C"
{
#endif


#ifndef EXCLUDE_FROM_DOCS

//  ==== Enumerations, structures, defines ====

/// Version information.
typedef struct {
    uint32_t                       api;   ///< API version (major.minor.rev: mmnnnrrrr dec).
    uint32_t                    kernel;   ///< Kernel version (major.minor.rev: mmnnnrrrr dec).
} osVersion_t;


/// Kernel state.
typedef enum {
    osKernelInactive        =  0,         ///< Inactive.
    osKernelReady           =  1,         ///< Ready.
    osKernelRunning         =  2,         ///< Running.
    osKernelLocked          =  3,         ///< Locked.
    osKernelSuspended       =  4,         ///< Suspended.
    osKernelError           = -1,         ///< Error.
    osKernelReserved        = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osKernelState_t;

#endif

/**
 * @addtogroup threading
 * @{
 */

/// Thread state.
typedef enum {
    osThreadInactive        =  0,         ///< Inactive.
    osThreadReady           =  1,         ///< Ready.
    osThreadRunning         =  2,         ///< Running.
    osThreadBlocked         =  3,         ///< Blocked.
    osThreadTerminated      =  4,         ///< Terminated.
    osThreadError           = -1,         ///< Error.
    osThreadReserved        = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osThreadState_t;

/// Priority values.
typedef enum {
    osPriorityNone          =  0,         ///< No priority (not initialized).
    osPriorityIdle          =  1,         ///< Reserved for Idle thread.
    osPriorityLow           =  8,         ///< Priority: low
    osPriorityLow1          =  8+1,       ///< Priority: low + 1
    osPriorityLow2          =  8+2,       ///< Priority: low + 2
    osPriorityLow3          =  8+3,       ///< Priority: low + 3
    osPriorityLow4          =  8+4,       ///< Priority: low + 4
    osPriorityLow5          =  8+5,       ///< Priority: low + 5
    osPriorityLow6          =  8+6,       ///< Priority: low + 6
    osPriorityLow7          =  8+7,       ///< Priority: low + 7
    osPriorityBelowNormal   = 16,         ///< Priority: below normal
    osPriorityBelowNormal1  = 16+1,       ///< Priority: below normal + 1
    osPriorityBelowNormal2  = 16+2,       ///< Priority: below normal + 2
    osPriorityBelowNormal3  = 16+3,       ///< Priority: below normal + 3
    osPriorityBelowNormal4  = 16+4,       ///< Priority: below normal + 4
    osPriorityBelowNormal5  = 16+5,       ///< Priority: below normal + 5
    osPriorityBelowNormal6  = 16+6,       ///< Priority: below normal + 6
    osPriorityBelowNormal7  = 16+7,       ///< Priority: below normal + 7
    osPriorityNormal        = 24,         ///< Priority: normal
    osPriorityNormal1       = 24+1,       ///< Priority: normal + 1
    osPriorityNormal2       = 24+2,       ///< Priority: normal + 2
    osPriorityNormal3       = 24+3,       ///< Priority: normal + 3
    osPriorityNormal4       = 24+4,       ///< Priority: normal + 4
    osPriorityNormal5       = 24+5,       ///< Priority: normal + 5
    osPriorityNormal6       = 24+6,       ///< Priority: normal + 6
    osPriorityNormal7       = 24+7,       ///< Priority: normal + 7
    osPriorityAboveNormal   = 32,         ///< Priority: above normal
    osPriorityAboveNormal1  = 32+1,       ///< Priority: above normal + 1
    osPriorityAboveNormal2  = 32+2,       ///< Priority: above normal + 2
    osPriorityAboveNormal3  = 32+3,       ///< Priority: above normal + 3
    osPriorityAboveNormal4  = 32+4,       ///< Priority: above normal + 4
    osPriorityAboveNormal5  = 32+5,       ///< Priority: above normal + 5
    osPriorityAboveNormal6  = 32+6,       ///< Priority: above normal + 6
    osPriorityAboveNormal7  = 32+7,       ///< Priority: above normal + 7
    osPriorityHigh          = 40,         ///< Priority: high
    osPriorityHigh1         = 40+1,       ///< Priority: high + 1
    osPriorityHigh2         = 40+2,       ///< Priority: high + 2
    osPriorityHigh3         = 40+3,       ///< Priority: high + 3
    osPriorityHigh4         = 40+4,       ///< Priority: high + 4
    osPriorityHigh5         = 40+5,       ///< Priority: high + 5
    osPriorityHigh6         = 40+6,       ///< Priority: high + 6
    osPriorityHigh7         = 40+7,       ///< Priority: high + 7
    osPriorityRealtime      = 48,         ///< Priority: realtime
    osPriorityRealtime1     = 48+1,       ///< Priority: realtime + 1
    osPriorityRealtime2     = 48+2,       ///< Priority: realtime + 2
    osPriorityRealtime3     = 48+3,       ///< Priority: realtime + 3
    osPriorityRealtime4     = 48+4,       ///< Priority: realtime + 4
    osPriorityRealtime5     = 48+5,       ///< Priority: realtime + 5
    osPriorityRealtime6     = 48+6,       ///< Priority: realtime + 6
    osPriorityRealtime7     = 48+7,       ///< Priority: realtime + 7
    osPriorityISR           = 56,         ///< Reserved for ISR deferred thread.
    osPriorityError         = -1,         ///< System cannot determine priority or illegal priority.
    osPriorityReserved      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osPriority_t;

/// Entry point of a thread.
typedef void (*osThreadFunc_t) (void *argument);

/** @} */

/**
 * @addtogroup core_timer
 * @{
 */

/// Timer callback function.
typedef void (*osTimerFunc_t) (void *argument);

/// Timer type.
typedef enum {
    osTimerOnce               = 0,          ///< One-shot timer.
    osTimerPeriodic           = 1           ///< Repeating timer.
} osTimerType_t;


/// Attributes structure for timer.
typedef struct {
    const char                   *name;   ///< name of the timer
    uint32_t                 attr_bits;   ///< attribute bits
    void                      *cb_mem;    ///< memory for control block
    uint32_t                   cb_size;   ///< size of provided memory for control block
} osTimerAttr_t;


/** @} */


/**
 * @addtogroup threading_threads
* @{
 */

// Timeout value.
#define osWaitForever         0xFFFFFFFFU ///< Wait forever timeout value.

/** @} */

/**
 * @addtogroup threading_eventflags
 * @{
 */

// Flags options (\ref osThreadFlagsWait and \ref osEventFlagsWait).
#define osFlagsWaitAny        0x00000000U ///< Wait for any flag (default).
#define osFlagsWaitAll        0x00000001U ///< Wait for all flags.
#define osFlagsNoClear        0x00000002U ///< Do not clear flags which have been specified to wait for.

// Flags errors (returned by osThreadFlagsXxxx and osEventFlagsXxxx).
#define osFlagsError          0x80000000U ///< Error indicator.
#define osFlagsErrorUnknown   0xFFFFFFFFU ///< osError (-1).
#define osFlagsErrorTimeout   0xFFFFFFFEU ///< osErrorTimeout (-2).
#define osFlagsErrorResource  0xFFFFFFFDU ///< osErrorResource (-3).
#define osFlagsErrorParameter 0xFFFFFFFCU ///< osErrorParameter (-4).
#define osFlagsErrorISR       0xFFFFFFFAU ///< osErrorISR (-6).

/** @} */

/**
 * @addtogroup threading_mutex
 * @{
 */

// Mutex attributes (attr_bits in \ref osMutexAttr_t).
#define osMutexRecursive      0x00000001U ///< Recursive mutex.
#define osMutexPrioInherit    0x00000002U ///< Priority inherit protocol.
#define osMutexRobust         0x00000008U ///< Robust mutex.

/** @} */

/// \details Status code values returned by threading functions
typedef enum {
    osOK                      =  0,         ///< Operation completed successfully.
    osError                   = -1,         ///< Unspecified RTOS error: run-time error but no other error message fits.
    osErrorTimeout            = -2,         ///< Operation not completed within the timeout period.
    osErrorResource           = -3,         ///< Resource not available.
    osErrorParameter          = -4,         ///< Parameter error.
    osErrorNoMemory           = -5,         ///< System is out of memory: it was impossible to allocate or reserve memory for the operation.
    osErrorISR                = -6,         ///< Not allowed in ISR context: the function cannot be called from interrupt service routines.
#ifndef EXCLUDE_FROM_DOCS
    osStatusReserved          = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
#endif
} osStatus_t;


/// \details Thread ID identifies the thread.
typedef void *osThreadId_t;

/// \details Timer ID identifies the timer.
typedef void *osTimerId_t;

/// \details Event Flags ID identifies the event flags.
typedef void *osEventFlagsId_t;

/// \details Mutex ID identifies the mutex.
typedef void *osMutexId_t;

/// \details Semaphore ID identifies the semaphore.
typedef void *osSemaphoreId_t;

/// \details Message Queue ID identifies the message queue.
typedef void *osMessageQueueId_t;


#ifndef TZ_MODULEID_T
#define TZ_MODULEID_T
/// \details Data type that identifies secure software modules called by a process.
typedef uint32_t TZ_ModuleId_t;
#endif

/**
 * @addtogroup threading_threads
* @{
 */


/// Attributes structure for thread.
typedef struct {
    const char                   *name;   ///< name of the thread
    uint32_t                 attr_bits;   ///< attribute bits
    void                      *cb_mem;    ///< memory for control block
    uint32_t                   cb_size;   ///< size of provided memory for control block
    void                   *stack_mem;    ///< memory for stack
    uint32_t                stack_size;   ///< size of stack
    osPriority_t              priority;   ///< initial thread priority (default: osPriorityNormal)
    TZ_ModuleId_t            tz_module;   ///< TrustZone module identifier
    uint32_t                  reserved;   ///< reserved (must be 0)
} osThreadAttr_t;

/** @} */

/**
 * @addtogroup threading_eventflags
 * @{
 */


/// Attributes structure for event flags.
typedef struct {
    const char                   *name;   ///< name of the event flags
    uint32_t                 attr_bits;   ///< attribute bits
    void                      *cb_mem;    ///< memory for control block
    uint32_t                   cb_size;   ///< size of provided memory for control block
} osEventFlagsAttr_t;

/** @} */

/**
 * @addtogroup threading_mutex
 * @{
 */

/// Attributes structure for mutex.
typedef struct {
    const char                   *name;   ///< name of the mutex
    uint32_t                 attr_bits;   ///< attribute bits
    void                      *cb_mem;    ///< memory for control block
    uint32_t                   cb_size;   ///< size of provided memory for control block
} osMutexAttr_t;

/** @} */

/**
 * @addtogroup threading_semaphore
 * @{
 */

// Attributes structure for semaphore.
typedef struct {
    const char                   *name;   ///< name of the semaphore
    uint32_t                 attr_bits;   ///< attribute bits
    void                      *cb_mem;    ///< memory for control block
    uint32_t                   cb_size;   ///< size of provided memory for control block
} osSemaphoreAttr_t;

/** @} */

/**
 * @addtogroup threading_queue
 * @{
 */

/// Attributes structure for message queue.
typedef struct {
    const char                   *name;   ///< name of the message queue
    uint32_t                 attr_bits;   ///< attribute bits
    void                      *cb_mem;    ///< memory for control block
    uint32_t                   cb_size;   ///< size of provided memory for control block
    void                      *mq_mem;    ///< memory for data storage
    uint32_t                   mq_size;   ///< size of provided memory for data storage
} osMessageQueueAttr_t;

/** @} */


/**
 * @defgroup threading_threads Threading
 * @brief Threading Functions
 * @details
 * @{
 */

//  ==== Thread Management Functions ====

/// \brief Create a thread and add it to Active Threads.
/// \param[in]     func          thread function.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \param[in]     attr          thread attributes; NULL: default values.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr);

/// \brief Get name of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return name as null-terminated string.
const char *osThreadGetName (osThreadId_t thread_id);

/// \brief Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadGetId (void);

/// \brief Get current thread state of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current thread state of the specified thread.
osThreadState_t osThreadGetState (osThreadId_t thread_id);

/// \brief Get stack size of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return stack size in bytes.
uint32_t osThreadGetStackSize (osThreadId_t thread_id);

/// \brief Get available stack space of a thread based on stack watermark recording during execution.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return remaining stack space in bytes.
uint32_t osThreadGetStackSpace (osThreadId_t thread_id);

/// \brief Change priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority);

/// \brief Get current priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current priority value of the specified thread.
osPriority_t osThreadGetPriority (osThreadId_t thread_id);

/// \brief Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadYield (void);

/// \brief Suspend execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSuspend (osThreadId_t thread_id);

/// \brief Resume execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadResume (osThreadId_t thread_id);

/// \brief Terminate execution of current running thread.
__NO_RETURN void osThreadExit (void);

/// \brief Terminate execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadTerminate (osThreadId_t thread_id);

/// \brief Get number of active threads.
/// \return number of active threads.
uint32_t osThreadGetCount (void);

/// \brief Enumerate active threads.
/// \param[out]    thread_array  pointer to array for retrieving thread IDs.
/// \param[in]     array_items   maximum number of items in array for retrieving thread IDs.
/// \return number of enumerated threads.
uint32_t osThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items);



//  ==== Thread Flags Functions ====

/// \brief Set the specified Thread Flags of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     flags         specifies the flags of the thread that shall be set.
/// \return thread flags after setting or error code if highest bit set.
uint32_t osThreadFlagsSet (osThreadId_t thread_id, uint32_t flags);

/// \brief Clear the specified Thread Flags of current running thread.
/// \param[in]     flags         specifies the flags of the thread that shall be cleared.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsClear (uint32_t flags);

/// \brief Get the current Thread Flags of current running thread.
/// \return current thread flags.
uint32_t osThreadFlagsGet (void);

/// \brief Wait for one or more Thread Flags of the current running thread to become signaled.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       the timeout in milliseconds or osWaitForever in case of no time-out. must be 0, if called in an interrupt context.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout);

/** @} */

/**
 * @addtogroup threading
 * @{
 */

/**
 * @defgroup threading_eventflags EventFlags
 * @brief EventFlag Functions
 * @details
 * @{
 */
//  ==== Event Flags Management Functions ====

/// \brief Create and Initialize an Event Flags object.
/// \param[in]     attr          event flags attributes; NULL: default values.
/// \return event flags ID for reference by other functions or NULL in case of error.
osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr);

/// \brief Get name of an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return name as null-terminated string.
const char *osEventFlagsGetName (osEventFlagsId_t ef_id);

/// \brief Set the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be set.
/// \return event flags after setting or error code if highest bit set.
uint32_t osEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags);

/// \brief Clear the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be cleared.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsClear (osEventFlagsId_t ef_id, uint32_t flags);

/// \brief Get the current Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return current event flags.
uint32_t osEventFlagsGet (osEventFlagsId_t ef_id);

/// \brief Wait for one or more Event Flags to become signaled.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout);

/// \brief Delete an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osEventFlagsDelete (osEventFlagsId_t ef_id);

/** @} */

/**
 * @defgroup threading_mutex Mutex
 * @brief Mutex functions
 * @details
 * @{
 */
//  ==== Mutex Management Functions ====

/// \brief Create and Initialize a Mutex object.
/// \param[in]     attr          mutex attributes; NULL: default values.
/// \return mutex ID for reference by other functions or NULL in case of error.
osMutexId_t osMutexNew (const osMutexAttr_t *attr);

/// \brief Get name of a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return name as null-terminated string.
const char *osMutexGetName (osMutexId_t mutex_id);

/// \brief Acquire a Mutex or timeout if it is locked.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout);

/// \brief Release a Mutex that was acquired by \ref osMutexAcquire.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexRelease (osMutexId_t mutex_id);

/// \brief Get Thread which owns a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return thread ID of owner thread or NULL when mutex was not acquired.
osThreadId_t osMutexGetOwner (osMutexId_t mutex_id);

/// \brief Delete a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexDelete (osMutexId_t mutex_id);

/** @} */

/**
 * @defgroup threading_semaphore Semaphores
 * @brief Semaphore functions
 * @details
 * @{
 */
//  ==== Semaphore Management Functions ====

/// \brief Create and Initialize a Semaphore object.
/// \param[in]     max_count     maximum number of available tokens.
/// \param[in]     initial_count initial number of available tokens.
/// \param[in]     attr          semaphore attributes; NULL: default values.
/// \return semaphore ID for reference by other functions or NULL in case of error.
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr);

/// \brief Get name of a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return name as null-terminated string.
const char *osSemaphoreGetName (osSemaphoreId_t semaphore_id);

/// \brief Acquire a Semaphore token or timeout if no tokens are available.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout);

/// \brief Release a Semaphore token up to the initial maximum count.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id);

/// \brief Get current Semaphore token count.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return number of tokens available.
uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id);

/// \brief Delete a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id);

/// \brief Rese a Semaphore to its initial state.
/// \param semaphore_id semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
uint32_t osSemaphoreReset (osSemaphoreId_t semaphore_id);

/** @} */

/**
 * @defgroup threading_queue MessageQueue
 * @brief Message Queue
 * @details
 * @{
 */


//  ==== Message Queue Management Functions ====


/// \brief Create and Initialize a Message Queue object.
/// \param[in]     msg_count     maximum number of messages in queue.
/// \param[in]     msg_size      maximum message size in bytes.
/// \param[in]     attr          message queue attributes; NULL: default values.
/// \return message queue ID for reference by other functions or NULL in case of error.
osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr);

/// \brief Get name of a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return name as null-terminated string.
const char *osMessageQueueGetName (osMessageQueueId_t mq_id);

/// \brief Put a Message into a Queue or timeout if Queue is full.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[in]     msg_ptr       pointer to buffer with message to put into a queue.
/// \param[in]     msg_prio      message priority.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);

/// \brief Get a Message from a Queue or timeout if Queue is empty.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
/// \param[out]    msg_prio      pointer to buffer for message priority or NULL.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);

/// \brief Get maximum number of messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum number of messages.
uint32_t osMessageQueueGetCapacity (osMessageQueueId_t mq_id);

/// \brief Get maximum message size in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum message size in bytes.
uint32_t osMessageQueueGetMsgSize (osMessageQueueId_t mq_id);

/// \brief Get number of queued messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of queued messages.
uint32_t osMessageQueueGetCount (osMessageQueueId_t mq_id);

/// \brief Get number of available slots for messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of available slots for messages.
uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id);

/// \brief Reset a Message Queue to initial empty state.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueReset (osMessageQueueId_t mq_id);

/// \brief  Delete a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueDelete (osMessageQueueId_t mq_id);

/** @} */

/** @} */


/**
 * @addtogroup core
 * @{
 */

#ifndef EXCLUDE_FROM_DOCS

//  ==== Kernel Management Functions ====

/// \brief  Initialize the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelInitialize (void);

/// \brief  Get RTOS Kernel Information.
/// \param[out]    version       pointer to buffer for retrieving version information.
/// \param[out]    id_buf        pointer to buffer for retrieving kernel identification string.
/// \param[in]     id_size       size of buffer for kernel identification string.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size);

/// \brief Get the current RTOS Kernel state.
/// \return current RTOS Kernel state.
osKernelState_t osKernelGetState (void);

/// \brief Start the RTOS Kernel scheduler.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelStart (void);

/// \brief Lock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelLock (void);

/// \brief Unlock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelUnlock (void);

/// \brief Restore the RTOS Kernel scheduler lock state.
/// \param[in]     lock          lock state obtained by \ref osKernelLock or \ref osKernelUnlock.
/// \return new lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelRestoreLock (int32_t lock);



/// \brief Get the RTOS kernel system timer count.
/// \return RTOS kernel current system timer count as 32-bit value.
uint32_t osKernelGetSysTimerCount (void);

/// \brief Get the RTOS kernel system timer frequency.
/// \return frequency of the system timer in hertz, i.e. timer ticks per second.
uint32_t osKernelGetSysTimerFreq (void);
#endif

/**
 * @addtogroup core_datetime
 * @{
 */

/// \brief Get the RTOS kernel tick count.
/// \return RTOS kernel current tick count.
uint32_t osKernelGetTickCount (void);

/// \brief Get the RTOS kernel tick frequency.
/// \return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
uint32_t osKernelGetTickFreq (void);

/** @} */

/** @} */


/**
 * @defgroup core_timer Timer & Delay
 * @brief Software millisecond/microsecond timer and delay functions
 * @details all "Timer" functions can be used to execute code periodically in millisecond intervals.
 *  Please be aware, that the timer thread runs at the lowest priority and uses the SysTick Interrupt
 *  to time thing. Therefore, this API should not be used for time-critical code that requires millisecond-precision
 *  but more for delayed one-time execution or periodic tasks that need to be performed, but don't
 *  suffer from a slight tolerance in the 1digit millisecond region.
 * @{
 */
//  ==== Timer Management Functions ====

/// \brief Create and Initialize a timer.
/// \param[in]     func          function pointer to callback function.
/// \param[in]     type          \ref osTimerOnce for one-shot or \ref osTimerPeriodic for periodic behavior.
/// \param[in]     argument      argument to the timer callback function.
/// \param[in]     attr          timer attributes; NULL: default values.
/// \return timer ID for reference by other functions or NULL in case of error.
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr);

/// \brief Get name of a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return name as null-terminated string.
const char *osTimerGetName (osTimerId_t timer_id);

/// \brief Start or restart a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \param[in]     ticks         millisecond tick value of the timer.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStart (osTimerId_t timer_id, uint32_t ticks);

/// \brief Stop a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStop (osTimerId_t timer_id);

/// \brief Check if a timer is running.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return 0 not running, 1 running.
uint32_t osTimerIsRunning (osTimerId_t timer_id);

/// \brief Delete a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerDelete (osTimerId_t timer_id);


//  ==== Generic Wait Functions ====

/// \brief Wait for Timeout (Time Delay).
/// \param[in]     ticks         milliseconds to delay current execution.
/// \return status code that indicates the execution status of the function.
osStatus_t osDelay(uint32_t ticks);

/// \brief Wait until specified time.
/// \param[in]     ticks         absolute time in ticks
/// \return status code that indicates the execution status of the function.
osStatus_t osDelayUntil(uint32_t ticks);

/// \brief Wait for Timeout (Microsecond Delay).
/// \param[in]     microseconds          microseconds to delay current execution
/// \return status code that indicates the execution status of the function.
osStatus_t osDelayUs(uint32_t microseconds);

/** @} */

#ifdef  __cplusplus
}


#endif

#endif  // CMSIS_OS2_H_