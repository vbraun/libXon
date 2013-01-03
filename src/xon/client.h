#ifndef XON_CLIENT__H
#define XON_CLIENT__H

/** @file */ 

#ifndef __cplusplus
#include <stdbool.h>
#endif 

#include <xon/object.h>

#ifdef __cplusplus
#define EXTERN_C_BEGIN  extern "C" {
#define EXTERN_C_END    }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN


/*******************************************************
 *
 * Client
 *
 *******************************************************/

/*! @brief IPC Client. 
 * 
 *  The client is you, that is, the side that is making the IPC
 *  request. You want to ask some other program to evaluate. The other
 *  program is the server. Everything in this struct is an
 *  implementation detail and subject to change without notice.
 */
typedef struct {
  /*! @brief The current buffer, can change if we need to realloc. */
  
} xon_client_struct;


/*! @brief Pointer to a xon_client_struct 
 *
 *  All public functions take / return such pointers, never the actual
 *  struct.
 */
typedef xon_client_struct *xon_client;


/*! @brief Create a new client
 *  
 *  @param dst name of the server program
 */
xon_client xon_client_new(const char *dst);


/*! @brief Destroy a client
 *
 *  @param client the client to destroy
 *
 *  You must call this function on every client that you created.
 */
void xon_client_delete(xon_client client);


/*! @brief Send a message
 *
 *  @param client the client to destroy
 *  @param obj the object to send
 *
 *  This call blocks until the obj is sent.
 */
xon_status xon_client_send(xon_client client, xon_obj obj);


/*! @brief Receive a message
 *
 *  @param client the client to destroy
 *  @param obj the object to receive
 *
 *  This call blocks until a new obj has been received.
 */
xon_status xon_client_recieve(xon_client client, xon_obj *obj);



EXTERN_C_END

#endif /* XON_CLIENT__H */
