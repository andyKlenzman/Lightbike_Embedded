/*
 * util.h
 *
 *  Created on: 23.02.2011
 *      Author: root
 */


/* 
What is this file??

.h files in general are used for naming and linking. By declaring that infact this name does exist, the 
linker is able to do it's job without throwing an error

My question, is why not just use a cpp file declaration, include it into your file, and 


 */

/* prevents files from being copied during linking */
#ifndef UTIL_H_
#define UTIL_H_

#include <inttypes.h>
#include <time.h>
#include <stdarg.h>
#include <string>

#include "FlakeConst.h"

#define MICROSEC_PER_SEC 1000000

using namespace std;

namespace flake
{
/* What is this declaration, where is the rest of the function? Is it a STD function  */
    void
    __usleep(long us); 

    extern "C" size_t xPortGetFreeHeapSize( void );
    extern "C" void flakePortableLog(const char* format, va_list args);

    typedef void * flakeMutexId_t;

    namespace utils
    {

        char*
        charFromString(string str);
        string
        stringFromChar(const char * str);
        uint32_t
        timestamp();
        char*
        copyStr(const char* src);
        unsigned char
        crc8(unsigned const char *data, unsigned char len);
    }



    struct flake_uuid
    {
      static void generate(uint8_t uuid[16]);
     private:
      static void read_node(uint8_t *node);
      static void read_random(uint8_t *out, uint8_t len);
      static uint64_t read_time(void);
    };

#if FLAKE_DEBUG_LOGGING
    const char*
    errorToString(long e);
    const char*
    timeToString(time_t t);
    const char*
    tagToString(uint32_t tag);

    const int lvlRaw = 10;
    const int lvlDebug = 9;
    const int lvlInfo = 5;
    const int lvlError = 2;
    const int lvlExplicit = 2;

    struct logging
    {
        static flakeMutexId_t _m;
        static bool m_inited;

        static int logLevel;


        static void init();

        template<int N>
        static void
        logf(const char* format, ...)
        {

          if (logLevel < N)
            return;
#if FLAKE_DEBUG_LOGGING
          va_list args;
          va_start(args, format);
          flakePortableLog(format, args);
          va_end(args);
          fflush(stdout);
#endif
        }

    };
#endif

}

#endif /* UTIL_H_ */
