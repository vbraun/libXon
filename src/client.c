#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "macros.h"
#include "xon/client.h"
#include "socket_comm.h"
#include "subprocess.h"

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
 * Client
 *
 *******************************************************/

const int buf_size = 8192;



EXPORTED_SYMBOL_C
xon_client xon_client_new(const char *dst)
{
  xon_client client = (xon_client)
    malloc(sizeof(xon_client_struct));
  if (client == NULL)
    return NULL;
  client->buf = (char*)malloc(buf_size);
  client->sockfd = server_listen();
  client->server_pid = run_subprocess(dst);
  client->sockfd = server_accept(client->sockfd);
  return client;
}


EXPORTED_SYMBOL_C
void xon_client_delete(xon_client client)
{
  free(client->buf);
  free(client);
}


EXPORTED_SYMBOL_C
xon_status xon_client_send(xon_client client, xon_obj obj)
{
  return socket_send_obj(client->sockfd, obj);
}


EXPORTED_SYMBOL_C
xon_status xon_client_receive(xon_client client, xon_obj *obj_ptr)
{
  return socket_recv_obj(client->sockfd, obj_ptr);
}




NAMESPACE_XON_C_API_END