

#include <libxon/object.h>



int main(void) {
  // We need only one object builder
  xon_obj_builder xs = xon_obj_builder_new();

  // Construct the first object
  xon_obj_builder_add("key", "value");
  // Build the object and reset the builder
  xon_obj obj = xon_obj_builder_to_obj();
  xon_obj_print(obj);
  // We are responsible to free the memory used by obj
  xon_obj_free(obj);  

  // Construct the second object, 
  xon_obj_stream_add("key", "newvalue");
  // Reuse the obj variable
  obj = xon_obj_stream_to_obj();
  xon_obj_print(obj, "Second object");
  // We are again responsible to free the memory used by obj
  xon_obj_free(obj);

  // Free the object builder that we used to build two objects
  xon_obj_stream_free(xs);

  return 0;
}
