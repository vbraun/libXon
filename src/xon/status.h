#ifndef XON__STATUS__H
#define XON__STATUS__H

/** @file */ 

#ifdef __cplusplus
#define XON__EXTERN_C_BEGIN  extern "C" {
#define XON__EXTERN_C_END    }
#else
#define XON__EXTERN_C_BEGIN
#define XON__EXTERN_C_END
#endif

XON__EXTERN_C_BEGIN


/*! @brief Status codes.
 */
typedef enum xon_stauts {
  /*! @brief Successful completion */
  XON_OK             = 0,
  /*! @brief Malloc returned NULL (likely out of memory) */
  XON_ERROR_MALLOC   = 1,

  XON_ERROR_SEND,
  XON_ERROR_RECV, 
  XON_TIMEOUT, 
} xon_status;



XON__EXTERN_C_END

#endif /* XON__STATUS__H */
