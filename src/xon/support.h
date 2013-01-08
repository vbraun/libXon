#ifndef XON__SUPPORT__H
#define XON__SUPPORT__H


#ifdef __cplusplus
#define XON__EXTERN_C_BEGIN  extern "C" {
#define XON__EXTERN_C_END    }
#define NAMESPACE_XON_C_API_BEGIN namespace xon { namespace c_api { 
#define NAMESPACE_XON_C_API_END } }
#else
#include <stdbool.h>
#define XON__EXTERN_C_BEGIN
#define XON__EXTERN_C_END
#define NAMESPACE_XON_C_API_BEGIN
#define NAMESPACE_XON_C_API_END
#endif




#endif /* XON__SUPPORT__H */
