#ifndef XON__PRELOAD_HACK__HH
#define XON__PRELOAD_HACK__HH

#include <stdio.h>
#include <stdarg.h>

#include "xon/support.h"

XON__EXTERN_C_BEGIN

int scanf(const char *format, ...);
int printf(const char *format, ...);

ssize_t read(int fildes, void *buf, size_t nbyte);
ssize_t read_unlocked(int fildes, void *buf, size_t nbyte);
ssize_t write(int fildes, const void *buf, size_t nbyte);




XON__EXTERN_C_END

#endif /* XON__PRELOAD_HACK__HH */

