#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "macros.h"
#include "xon/server.h"
#include "socket_comm.h"

NAMESPACE_XON_C_API_BEGIN

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
    malloc(sizeof(xon_server_struct));
  if (server == NULL)
    return NULL;
  server->sockfd = client_connect();
  return server;
}



EXPORTED_SYMBOL_C
void xon_server_delete(xon_server server)
{
  free(server);
}


EXPORTED_SYMBOL_C
xon_status xon_server_send(xon_server server, xon_obj obj)
{
  int n = socket_send_all(server->sockfd, obj, xon_obj_size(obj));
  if (n == -1) 
    return XON_ERROR_SEND;
  return XON_OK;
}


EXPORTED_SYMBOL_C
xon_status xon_server_receive(xon_server server, xon_obj *obj_ptr)
{
  int n = socket_recv_all(server->sockfd, obj_ptr);
  if (n == -1)
    return XON_ERROR_RECV;
  assert(n == xon_obj_size(*obj_ptr));
  return XON_OK;
}




NAMESPACE_XON_C_API_END
