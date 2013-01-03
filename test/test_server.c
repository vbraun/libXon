#include <stdio.h>

#include <xon/object.h>
#include <xon/server.h>


int main(void)
{
  xon_server server = xon_server_new();



  xon_server_delete(server);
  
  return 0;
}
