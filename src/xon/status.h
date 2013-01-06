#ifndef XON__STATUS__H
#define XON__STATUS__H

/** @file */ 

#include <xon/support.h>

NAMESPACE_XON_C_API_BEGIN




/*! @brief Status codes.
 */
typedef enum xon_status {
  /*! @brief Successful completion */
  XON_OK             = 0,
  /*! @brief Malloc returned NULL (likely out of memory) */
  XON_ERROR_MALLOC   = 1,
  
  XON_ERROR_ARCH_MISMATCH,

  XON_ERROR_SEND,
  XON_ERROR_RECV, 
  XON_TIMEOUT, 
} xon_status;



NAMESPACE_XON_C_API_END

#endif /* XON__STATUS__H */
