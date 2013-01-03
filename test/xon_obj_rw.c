#include <stdio.h>
#include <malloc.h>

#include <xon/object.h>



int main(void) {
  // We need only one object builder
  xon_obj_builder xb = xon_obj_builder_new();

  // Construct the first object
  xon_obj_builder_add_string(xb, "key", "value");
  xon_obj_builder_add_string(xb, "key2", "value2");
  xon_obj_builder_add_string(xb, "key3", "value3");
  xon_obj_builder_add_double(xb, "float", 3.1415);
  xon_obj_builder_add_int32 (xb, "int",   123);
  xon_obj_builder_add_int64 (xb, "large", (int64_t)(1) << 40);
  // Build the object and reset the builder
  void* obj = xon_obj_builder_get(xb);
  printf("\nFirst object\n");
  xon_obj_print(obj);
  printf("Size = %d bytes\n\n", xon_obj_size(obj));
  // We are responsible to free the memory used by obj
  free(obj);  

  // Construct the second object, 
  xon_obj_builder_add_string(xb, "key",   "0123456");
  xon_obj_builder_add_string(xb, "key2",  "01234567");
  xon_obj_builder_add_string(xb, "key3",  "012345678");
  xon_obj_builder_add_string(xb, "key4",  "newvalue4");
  xon_obj_builder_add_double(xb, "float", 3.1415);
  xon_obj_builder_add_int32 (xb, "int",   123);
  xon_obj_builder_add_int64 (xb, "large", (int64_t)(1) << 40);
  // Reuse the obj variable
  obj = xon_obj_builder_get(xb);
  printf("Second object\n");
  xon_obj_print(obj);
  printf("Size = %d bytes\n\n", xon_obj_size(obj));
  // We are again responsible to free the memory used by obj
  free(obj);

  // Delete the object builder that we used to build two objects
  xon_obj_builder_delete(xb);

  return 0;
}
