/*
 * macro.h
 *
 *  Created on: 23.01.2011
 *      Author: Sebastian
 */

#ifndef MACRO_H_
#define MACRO_H_

#ifdef _WIN32
#define 
#define DLLEXPORT
typedef unsigned int uint;

//#define  __stdcall
//#define DLLEXPORT extern "C"  __declspec(dllexport)
#else
#define DLLCALL
#define DLLEXPORT
#endif

#define UNUSED(expr) do { (void)(expr); } while (0)

#endif /* MACRO_H_ */
