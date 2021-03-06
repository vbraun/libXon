#ifndef XON__CLIENT__H
#define XON__CLIENT__H

#include <xon/support.h>

/** @file */ 

#ifndef __cplusplus
#include <stdbool.h>
#endif 

#include <sys/types.h>

#include <xon/status.h>
#include <xon/object.h>

NAMESPACE_XON_C_API_BEGIN



/*******************************************************
 *
 * Client
 *
 *******************************************************/

/*! @brief Local IPC Client. 
 * 
 *  The client is you, that is, the side that is making the IPC
 *  request. You want to ask some other program to evaluate. The other
 *  program is the server. Everything in this struct is an
 *  implementation detail and subject to change without notice.
 */
typedef struct {
  /*! @brief The current buffer, can change if we need to realloc. */
  char *buf;
  int sockfd;
  pid_t server_pid;
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
xon_client xon_client_new(const char *dst, const char *argv[]);


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
xon_status xon_client_receive(xon_client client, xon_obj *obj);


/*! @brief Wait for the server to quit
 *
 *  @param client the client (you) that is connected to the server
 *  @param timeout the timeout it seconds
 *
 *  This call blocks until a new obj has been received.
 */
xon_status xon_client_wait(xon_client client, double timeout);

/*! @brief Kill spawned server 
 *
 *  @param client the client (you)
 */
void xon_client_kill(xon_client client);




NAMESPACE_XON_C_API_END

#endif /* XON__CLIENT__H */
