#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <unistd.h>

#include "macros.h"
#include "xon/server.h"
#include "socket_comm.h"

NAMESPACE_XON_C_API_BEGIN


/*! @brief IPC Server. 
 * 
 *  The server is some program that you want to run (or evaluate a
 *  command in) and get the result back into your own
 *  session. Everything in this struct is an implementation detail and
 *  subject to change without notice.
 */
struct xon_server {
  /*! @brief Socket to communicate with client */
  int sockfd;
};


/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/


CONSTRUCTOR_C
void init() 
{
}


DESTRUCTOR_C
void fini() 
{
}


/*******************************************************
 *
 * Server
 *
 *******************************************************/

EXPORTED_SYMBOL_C
xon_server xon_server_new()
{
  xon_server server = (xon_server)
    malloc(sizeof(struct xon_server));
  if (server == NULL)
    return NULL;
  server->sockfd = client_connect();
  if (server->sockfd == -1) {
    xon_server_delete(server);
    return NULL;
  }
  return server;
}



EXPORTED_SYMBOL_C
void xon_server_delete(xon_server server)
{
  if (server->sockfd >= 0)
    close(server->sockfd);
  free(server);
}


EXPORTED_SYMBOL_C
xon_status xon_server_send(xon_server server, xon_obj obj)
{
  return socket_send_obj(server->sockfd, obj);
}


EXPORTED_SYMBOL_C
xon_status xon_server_receive(xon_server server, xon_obj *obj_ptr)
{
  return socket_recv_obj(server->sockfd, obj_ptr);
}




NAMESPACE_XON_C_API_END