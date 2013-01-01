#ifndef XON_MACROS__H
#define XON_MACROS__H

#include "config.h"


#if BUILDING_XON_LIBRARY && HAVE_VISIBILITY
  #define XON_EXPORTED_SYMBOL __attribute__((__visibility__("default")))
#elif defined _MSC_VER
  #define XON_EXPORTED_SYMBOL __declspec(dllexport)
#elif defined _MSC_VER
  #define XON_EXPORTED_SYMBOL __declspec(dllimport)
#else
  #define XON_EXPORTED_SYMBOL
#endif


#define XON_CONSTRUCTOR __attribute__((constructor)) 
#define XON_DESTRUCTOR  __attribute__((destructor))  


#endif /* XON_MACROS__H */
