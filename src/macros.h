#ifndef XON__MACROS__H
#define XON__MACROS__H

#include "config.h"
#include "align.h"

// Build the C version of the library

#if BUILDING_XON_LIBRARY_C 
#if HAVE_VISIBILITY
#define EXPORTED_SYMBOL_C __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_C __declspec(dllexport)
#else
#define EXPORTED_SYMBOL_C
#endif
#endif

#if BUILDING_XON_LIBRARY_C
#define CONSTRUCTOR_C __attribute__((constructor)) 
#define DESTRUCTOR_C  __attribute__((destructor))  
#endif

#if BUILDING_XON_LIBRARY_C
#define EXPORTED_SYMBOL_CPP error()
#define CONSTRUCTOR_CPP error()
#define DESTRUCTOR_CPP error()
#endif


// Build the C++ version of the library

#if BUILDING_XON_LIBRARY_CPP
#if HAVE_VISIBILITY
#define EXPORTED_SYMBOL_CPP __attribute__((__visibility__("default")))
#define EXPORTED_SYMBOL_C __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define EXPORTED_SYMBOL_CPP __declspec(dllexport)
#define EXPORTED_SYMBOL_C __declspec(dllexport)
#else
#define EXPORTED_SYMBOL_CPP
#define EXPORTED_SYMBOL_C
#endif
#endif

#ifdef BUILDING_XON_LIBRARY_CPP
#define CONSTRUCTOR_CPP __attribute__((constructor)) 
#define DESTRUCTOR_CPP  __attribute__((destructor))  
#endif

#ifdef BUILDING_XON_LIBRARY_CPP
#define CONSTRUCTOR_C
#define DESTRUCTOR_C
#endif






#endif /* XON__MACROS__H */
