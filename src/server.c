#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <unistd.h>

#include "macros.h"
#include "xon/server.h"
#include "socket_comm.h"
#include "cookie.h"

NAMESPACE_XON_C_API_BEGIN


/*! @brief IPC Server. 
 * 
 *  The server is some program that you want to run (or evaluate a
 *  command in) and get the result back into your own
 *  session. Everything in this struct is an implementation detail and
 *  subject to change without notice.
 */
struct xon_server_struct {
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
    malloc(sizeof(struct xon_server_struct));
  bool good = (server != NULL);
  
  if (good) {
    server->sockfd = -1;
  }

  const char *cookie = get_cookie_from_environ();
  good = good && (cookie != NULL);
    
  int cookie_size;
  if (good) {
    cookie_size = strlen(cookie)+1;
    server->sockfd = client_connect(cookie);
    good = (server->sockfd != -1);
  }

  if (good) {
    xon_status rc = socket_send(server->sockfd, cookie, cookie_size);
    good = (rc == XON_OK);
  }
  
  if (good)
    return server;
  else {
    xon_server_delete(server);
    return NULL;
  }
}



EXPORTED_SYMBOL_C
void xon_server_delete(xon_server server)
{
  if (server == NULL)
    return;
  if (server->sockfd != -1)
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
