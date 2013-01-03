#ifndef XON_MACROS__H
#define XON_MACROS__H

#include "align.h"

#if BUILDING_XON_LIBRARY && HAVE_VISIBILITY
#define EXPORTED_SYMBOL_CPP __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_CPP __declspec(dllexport)
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_CPP __declspec(dllimport)
#else
#define EXPORTED_SYMBOL_CPP
#endif


#define CONSTRUCTOR_CPP __attribute__((constructor)) 
#define DESTRUCTOR_CPP  __attribute__((destructor))  


#define EXPORTED_SYMBOL_C
#define CONSTRUCTOR_C
#define DESTRUCTOR_C


#endif /* XON_MACROS__H */
