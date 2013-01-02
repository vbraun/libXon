#ifndef XON_MACROS__H
#define XON_MACROS__H

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


#if BUILDING_XON_LIBRARY && HAVE_VISIBILITY
#define EXPORTED_SYMBOL __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define EXPORTED_SYMBOL __declspec(dllexport)
#elif defined _MSC_VER
#define EXPORTED_SYMBOL __declspec(dllimport)
#else
#define EXPORTED_SYMBOL
#endif


#define CONSTRUCTOR __attribute__((constructor)) 
#define DESTRUCTOR  __attribute__((destructor))  


#endif /* XON_MACROS__H */
