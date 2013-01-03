#include <stdio.h>
#include <stdlib.h>

#include <xon/client.h>



xon_obj make_obj()
{
  xon_obj_builder xb = xon_obj_builder_new();
  xon_obj_builder_add_string(xb, "key",  "avalue");
  xon_obj_builder_add_string(xb, "key2", "avalue2");
  xon_obj_builder_add_string(xb, "key3", "abdcefgh");
  xon_obj_builder_add_double(xb, "float", 3.1415);
  xon_obj_builder_add_int32 (xb, "int",   123);
  xon_obj_builder_add_int64 (xb, "large", (int64_t)(1) << 40);
  xon_obj obj = xon_obj_builder_get(xb);
  xon_obj_builder_delete(xb);
  return obj;
}



int main(void)
{
  xon_client client = xon_client_new("test_server_c");
  
  xon_obj obj = make_obj();
  printf("Sending object");
  xon_client_send(client, obj);
  free(obj);

  xon_client_receive(client, &obj);
  printf("Received object");
  xon_obj_print(obj);
  free(obj);
  
  xon_client_delete(client);
  return 0;
}
