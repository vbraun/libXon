#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>

#include "macros.h"
#include "xon/status.h"
#include "xon/object.h"
#include "xon/client.h"
#include "socket_comm.h"
#include "subprocess.h"
#include "cookie.h"

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

EXPORTED_SYMBOL_C
xon_client xon_client_new(const char *dst)
{
  xon_client client = (xon_client)
    malloc(sizeof(xon_client_struct));
  bool good = (client != NULL);
  int port = 12345;

  if (good) {
    client->server_pid = 0;
    client->sockfd = server_listen_net(&port);
    good = (client->sockfd != -1);
  }

  char* cookie = NULL;
  if (good) {
    cookie = new_cookie(port);
    good = (cookie != NULL);
  }

  if (good) {
    client->server_pid = run_subprocess(dst, cookie);
    good = (client->server_pid != 0);
  }

  if (good) {
    client->sockfd = server_accept(client->sockfd, cookie);
    good = (client->sockfd != -1);
  }

  int cookie_size;
  char *received_cookie = NULL;
  if (good) {
    cookie_size = strlen(cookie) + 1;
    received_cookie = (char*)malloc(cookie_size);
    good = (received_cookie != NULL);
  }

  if (good) {
    xon_status rc = socket_recv_buffer(client->sockfd, received_cookie, cookie_size);
    good = (rc == XON_OK) && (strcmp(cookie, received_cookie) == 0);
  }

  free(received_cookie);
  free(cookie);
  if (good)
    return client;
  else {
    xon_client_delete(client);
    return NULL;
  }
}


EXPORTED_SYMBOL_C
void xon_client_delete(xon_client client)
{
  if (client == NULL)
    return;
  xon_client_kill(client);
  if (client->sockfd != -1)
    close(client->sockfd);
  free(client);
}


EXPORTED_SYMBOL_C
void xon_client_kill(xon_client client)
{
  if (client->server_pid == 0)
    return;
  kill_subprocess(client->server_pid);
}


EXPORTED_SYMBOL_C
xon_status xon_client_wait(xon_client client, double timeout)
{
  return wait_for_subprocess(client->server_pid, timeout);
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
