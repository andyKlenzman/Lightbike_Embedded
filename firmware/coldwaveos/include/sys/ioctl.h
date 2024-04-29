/**
 * @file
 * This file is a drop in replacement for ioctl.h on
 * systems that lack this file
 */

#ifndef _IOCTL_H
#define _IOCTL_H

#if defined (__cplusplus)
extern "C" {
#endif

int ioctl (int s, long cmd, void *argp);

#if defined (__cplusplus)
}
#endif


#endif //_IOCTL_H