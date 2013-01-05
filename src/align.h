#ifndef XON__ALIGN_BYTES__H
#define XON__ALIGN_BYTES__H

#include "config.h"


/* ALIGN_BYTES is a power of two */
/* ALIGN_BYTES_LOG2 is its log_2 */


#if ALIGNOF_DOUBLE > ALIGNOF_INT
#define ALIGN_BYTES ALIGNOF_DOUBLE
#else
#define ALIGN_BYTES ALIGNOF_INT
#endif


#if ALIGN_BYTES == 4
#define ALIGN_BYTES_LOG2 2   /* 32 bit */
#elif ALIGN_BYTES == 8
#define ALIGN_BYTES_LOG2 3   /* 64 bit */
#elif ALIGN_BYTES == 16
#define ALIGN_BYTES_LOG2 4   /* 128 bit */
#else
#error "Unknown alignment"
#endif


#define ALIGN_ROUND_UP_PTR(x) \
  (char*)(((uintptr_t)x + ALIGN_BYTES - 1) & ~ (ALIGN_BYTES-1))

#define ALIGN_ROUND_UP_SIZE(x) \
  (((size_t)x + ALIGN_BYTES - 1) & ~ (ALIGN_BYTES-1))


#ifdef WORDS_BIGENDIAN
#define XON_MAGIC -(ALIGN_BYTES_LOG2 + 0x1000)
#else
#define XON_MAGIC -(ALIGN_BYTES_LOG2)
#endif



#endif  /* XON__ALIGN_BYTES__H */
