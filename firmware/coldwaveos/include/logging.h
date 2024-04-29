/*******************************************************************************
 * @file       logging.h
 * @brief
 * @copyright  Copyright (c) 2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef _LOGGING_H_
#define _LOGGING_H_

#if defined (__cplusplus)
extern "C" {
#endif

#define CW_LOGLVL_SEVERE  (0)
#define CW_LOGLVL_ERROR   (1)
#define CW_LOGLVL_WARNING (2)
#define CW_LOGLVL_INFO    (3)
#define CW_LOGLVL_DEBUG   (4)

#define LOG_MODULE(x) \
   static const char* cw_log_module __attribute__((unused)) = #x ;
#define LOG_MODULE_REREG(x) \
   cw_log_module = #x;

#define LOG_DEBUG(...) CW_LOG_FORMATTED(CW_LOGLVL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) CW_LOG_FORMATTED(CW_LOGLVL_INFO, __VA_ARGS__)
#define LOG_WARNING(...) CW_LOG_FORMATTED(CW_LOGLVL_WARNING, __VA_ARGS__)
#define LOG_ERROR(...) CW_LOG_FORMATTED(CW_LOGLVL_ERROR, __VA_ARGS__)
#define LOG_SEVERE(...) CW_LOG_FORMATTED(CW_LOGLVL_SEVERE, __VA_ARGS__)

#define CW_LOG_FORMATTED(lvl, ...) { \
    CW_LOG_PREPARE(lvl)              \
    CW_LOG_TIMESTAMP                 \
    CW_LOG_PREFIX(lvl, cw_log_module)\
    CW_LOG_OUTP(__VA_ARGS__)  }      \
    CW_LOG_CLEANUPN

#define CW_LOG_PREPARE(l)     cw_log_prepare_formatting(l);
#define CW_LOG_PREFIX(l, m)    cw_log_prefix(l,m);
#define CW_LOG_TIMESTAMP      cw_log_timestamp();
#define CW_LOG_OUTP(...)      cw_log_output(__VA_ARGS__);
#define CW_LOG_CLEANUPN       cw_log_cleanup(1)

void __attribute__((format (printf, 1, 2))) cw_log_output (const char *fmt, ...);

void cw_log_prepare_formatting (int lvl);

void cw_log_prefix (int lvl, const char *mod);

void cw_log_cleanup (int nl);

void cw_log_timestamp (void);

#if defined (__cplusplus)
}
#endif

#endif //_LOGGING_H_
