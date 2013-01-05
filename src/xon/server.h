#ifndef XON__SERVER__H
#define XON__SERVER__H

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
 * Server
 *
 *******************************************************/

/*! @brief Pointer to a xon_server_struct 
 *
 *  All public functions take / return such pointers, never the actual
 *  struct.
 */
typedef struct xon_server *xon_server;


/*! @brief Create a new server
 */
xon_server xon_server_new();


/*! @brief Destroy a server
 *
 *  @param server the server to destroy
 *
 *  You must call this function on every server that you created.
 */
void xon_server_delete(xon_server server);


/*! @brief Send a message
 *
 *  @param server the server to send from
 *  @param obj the object to send
 *
 *  This call blocks until the obj is sent.
 */
xon_status xon_server_send(xon_server server, xon_obj obj);


/*! @brief Receive a message
 *
 *  @param server the server to receive the request
 *  @param obj the object to receive
 *
 *  This call blocks until a new obj has been received.
 */
xon_status xon_server_receive(xon_server server, xon_obj *obj_ptr);



EXTERN_C_END

#endif /* XON__SERVER__H */