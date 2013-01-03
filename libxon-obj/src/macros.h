#ifndef XON_MACROS__H
#define XON_MACROS__H

#include "align.h"

// Build the C version of the library

#if BUILDING_XON_LIBRARY_C && HAVE_VISIBILITY
#define EXPORTED_SYMBOL_C __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_C __declspec(dllexport)
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_C __declspec(dllimport)
#else
#define EXPORTED_SYMBOL_C
#endif

#if BUILDING_XON_LIBRARY_C
#define CONSTRUCTOR_C __attribute__((constructor)) 
#define DESTRUCTOR_C  __attribute__((destructor))  
#else
#define CONSTRUCTOR_C
#define DESTRUCTOR_C
#endif

#if BUILDING_XON_LIBRARY_C
#define EXPORTED_SYMBOL_CPP error()
#define CONSTRUCTOR_CPP error()
#define DESTRUCTOR_CPP error()
#endif


// Build the C++ version of the library

#if BUILDING_XON_LIBRARY_CPP && HAVE_VISIBILITY
#define EXPORTED_SYMBOL_CPP __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_CPP __declspec(dllexport)
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_CPP __declspec(dllimport)
#else
#define EXPORTED_SYMBOL_CPP
#endif

#if BUILDING_XON_LIBRARY_CPP
#define CONSTRUCTOR_CPP __attribute__((constructor)) 
#define DESTRUCTOR_CPP  __attribute__((destructor))  
#else
#define CONSTRUCTOR_CPP
#define DESTRUCTOR_CPP
#endif

#if BUILDING_XON_LIBRARY_CPP
#define EXPORTED_SYMBOL_C
#define CONSTRUCTOR_C
#define DESTRUCTOR_C
#endif




#endif /* XON_MACROS__H */
