#include <stdio.h>
#include <stdlib.h>

#include <xon/object.h>
#include <xon/server.h>


int main(void)
{
  xon_server server = xon_server_new();

  xon_obj obj;
  xon_server_receive(server, &obj);
  xon_server_send(server, obj);
  free(obj);
  
  xon_server_delete(server);
  return 0;
}
