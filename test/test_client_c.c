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








int test(const char* command)
{
  xon_client client = xon_client_new("./test_server_c", NULL);
  if (client == NULL) {
    printf("Client: Error starting client.\n");
    return 1;
  }

  xon_obj obj = make_obj();
  if (obj == NULL) {
    printf("Client: Error creating binary object.\n");
    return 1;
  }

  printf("\nClient: Sending:\n");
  xon_obj_print(obj);
  if (xon_client_send(client, obj) != XON_OK) {
    printf("Client: Error sending message.\n");
    return 1;
  }
  free(obj);

  if (xon_client_receive(client, &obj) != XON_OK) {
    printf("Client: Error receiving message.\n");
    return 2;
  }

  printf("\nReceived object:\n");
  xon_obj_print(obj);
  free(obj);

  if (xon_client_wait(client, 1000) != XON_OK) {
    printf("Server did not quit by himself.\n");
    /* xon_client_delete will kill server */
  }
  
  xon_client_delete(client);
  return 0;
}


int main(void)
{
  int rc = test("./test_server_c");
  if (rc != 0)
    return rc;
  rc = test("./test_server_cxx");
  return rc;
}
