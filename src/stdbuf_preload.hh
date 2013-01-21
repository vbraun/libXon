#ifndef XON__STDBUF_PRELOAD_HACK__HH
#define XON__STDBUF_PRELOAD_HACK__HH

/** @file */ 

#include <stdio.h>

//! Make the program believe that it is talking to a TTY
int isatty(int fd);

//! Override setbuf to use line buffer mode
void setbuf(FILE *stream, char *buf);

//! Override setbuffer to use line buffer mode
void setbuffer(FILE *stream, char *buf, size_t size);

//! Override linebuf to use line buffer mode
void setlinebuf(FILE *stream);

//! Override setvbuf to use line buffer mode
int setvbuf(FILE *stream, char *buf, int mode, size_t size);

#endif /* XON__STDBUF_PRELOAD_HACK__HH */

