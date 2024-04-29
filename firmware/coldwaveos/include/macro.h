/*******************************************************************************
 * @file       macro.h
 * @brief
 * @copyright  Copyright (c) 2018-2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef _COLDWAVEOS_MACRO_H
#define _COLDWAVEOS_MACRO_H

#define c_container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define  SECOND(a, b, ...) b

#define  IS_PROBE(...) SECOND(__VA_ARGS__,  )
#define  PROBE() ~, NOT_EMPTY

#define  CAT(a,b) a ## b

#define  NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_ PROBE()

#define  BOOL(x) NOT(NOT(x))

#define  IF(condition) _IF(BOOL(condition))
#define _IF(condition) CAT(_IF_, condition)

#define _IF_NOT_EMPTY(...)
#define _IF_(...)	__VA_ARGS__

#define ASSERT(X)
#define Assert(X)

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

#ifndef UNUSED
#define UNUSED(expr) do { (void)(expr); } while (0)
#endif

#define DLLCALL
#define DLLEXPORT

#define BYTE_FORMAT "%.2hX"
#define BYTE_SEPARATOR " "

#define BYTES_2_FORMAT_SEPARATOR(SEPARATOR) BYTE_FORMAT SEPARATOR BYTE_FORMAT
#define BYTES_4_FORMAT_SEPARATOR(SEPARATOR) BYTES_2_FORMAT_SEPARATOR(SEPARATOR) SEPARATOR BYTES_2_FORMAT_SEPARATOR(SEPARATOR)
#define BYTES_8_FORMAT_SEPARATOR(SEPARATOR) BYTES_4_FORMAT_SEPARATOR(SEPARATOR) SEPARATOR BYTES_4_FORMAT_SEPARATOR(SEPARATOR)
#define BYTES_16_FORMAT_SEPARATOR(SEPARATOR) BYTES_8_FORMAT_SEPARATOR(SEPARATOR) SEPARATOR BYTES_8_FORMAT_SEPARATOR(SEPARATOR)
#define BYTES_32_FORMAT_SEPARATOR(SEPARATOR) BYTES_16_FORMAT_SEPARATOR(SEPARATOR) SEPARATOR BYTES_16_FORMAT_SEPARATOR(SEPARATOR)

#define BYTES_2_FORMAT BYTES_2_FORMAT_SEPARATOR(BYTE_SEPARATOR)
#define BYTES_4_FORMAT BYTES_4_FORMAT_SEPARATOR(BYTE_SEPARATOR)
#define BYTES_8_FORMAT BYTES_8_FORMAT_SEPARATOR(BYTE_SEPARATOR)
#define BYTES_16_FORMAT BYTES_16_FORMAT_SEPARATOR(BYTE_SEPARATOR)
#define BYTES_32_FORMAT BYTES_32_FORMAT_SEPARATOR(BYTE_SEPARATOR)

#define BYTES_2(bytes) (bytes)[0], (bytes)[1]
#define BYTES_4(bytes) BYTES_2(bytes), BYTES_2(bytes + 2)
#define BYTES_8(bytes) BYTES_4(bytes), BYTES_4(bytes + 4)
#define BYTES_16(bytes) BYTES_8(bytes), BYTES_8(bytes + 8)
#define BYTES_32(bytes) BYTES_16(bytes), BYTES_16(bytes + 16)

#define IP32_TO_BYTES_FORMAT BYTES_4_FORMAT_SEPARATOR(".")

#define BIT32_TO_BYTES(bits) BYTES_4((uint8_t*) &(bits))
#define IP32_TO_BYTES(ip) BIT32_TO_BYTES(ip)


#define AES_BLOCK_TO_BYTES_FORMAT BYTES_16_FORMAT
#define AES_BLOCK_TO_BYTES(block) BIT32_TO_BYTES((block)[0]), BIT32_TO_BYTES((block)[1]), BIT32_TO_BYTES((block)[2]), BIT32_TO_BYTES((block)[3])



#endif //_COLDWAVEOSMACRO_H
