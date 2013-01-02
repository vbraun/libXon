#include <stdio.h>
#include <malloc.h>

#include <xon/object.h>



int main(void) {
  // We need only one object builder
  xon_obj_builder xb = xon_obj_builder_new();

  // Construct the first object
  xon_obj_builder_add_string(xb, "key", "value");
  // Build the object and reset the builder
  void* obj = xon_obj_builder_finish(xb);
  printf("First object");
  xon_obj_print(obj);
  // We are responsible to free the memory used by obj
  free(obj);  

  // Construct the second object, 
  xon_obj_builder_add_string(xb, "key", "newvalue");
  // Reuse the obj variable
  obj = xon_obj_builder_finish(xb);
  printf("Second object");
  xon_obj_print(obj);
  // We are again responsible to free the memory used by obj
  free(obj);

  // Free the object builder that we used to build two objects
  xon_obj_builder_free(xb);

  return 0;
}
