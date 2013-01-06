#include <stdio.h>
#include <stdlib.h>

#include <xon/object.h>
#include <xon/server.h>


xon_obj make_obj()
{
  xon_obj_builder xb = xon_obj_builder_new();
  xon_obj_builder_add_string(xb, "new",  "avalue");
  xon_obj_builder_add_string(xb, "new2", "avalue2");
  xon_obj_builder_add_string(xb, "new3", "abdcefgh");
  xon_obj_builder_add_double(xb, "number", 3.1415926535);
  xon_obj_builder_add_int32 (xb, "integer",   124);
  xon_obj_builder_add_int64 (xb, "long", (int64_t)(1) << 41);
  xon_obj obj = xon_obj_builder_get(xb);
  xon_obj_builder_delete(xb);
  return obj;
}


int main(void)
{
  printf("Server: startup\n");
  xon_server server = xon_server_new();
  if (server == NULL) {
    printf("Server: startup failed.\n");
    return 3;
  }

  xon_obj obj;

  printf("Server: receiving message\n");
  if (xon_server_receive(server, &obj) != XON_OK) {
    printf("Server: Error receiving message.\n");
    return 4;
  }
  printf("Server: received message\n");

  free(obj);
  obj = make_obj();

  printf("Server: sending reply\n");
  if (xon_server_send(server, obj) != XON_OK) {
    printf("Server: Error sending message.\n");
    return 5;
  }
  printf("Server: sent reply\n");
  free(obj);
  
  xon_server_delete(server);
  return 0;
}
