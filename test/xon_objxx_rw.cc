#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <xon/object.hh>



void test_c_api()
{
  using namespace xon::c_api;
  
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
  xon_obj obj = xon_obj_builder_get(xb);
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
}


void test_cplusplus_api() 
{
  using std::cout;
  using std::endl;

  // We need only one object builder
  xon::obj_builder xb;
  xb.get();

  // Construct the first object
  xb.add("key", "value");
  xb.add("key2", "value2");
  xb.add("key3", "value3");
  xb.add("float", 3.1415);
  xb.add("int",   123);
  xb.add("large", (int64_t)(1) << 40);
  // Build the object and reset the builder
  xon::object obj(xb);
  cout << endl << "First object" 
       << endl << obj
       << "Size = " << obj.size() << " bytes" 
       << endl << endl;

  // Construct the second object, 
  xb.add("key",   "0123456");
  xb.add("key2",  "01234567");
  xb.add("key3",  "012345678");
  xb.add("key4",  "newvalue4");
  xb.add("float", 3.1415);
  xb.add("int",   123);
  xb.add("large", (int64_t)(1) << 40);
  obj = xb.get();
  cout << "Second object" 
       << endl << obj
       << "Size = " << obj.size() << " bytes" 
       << endl << endl;

  const xon::obj_reader xr = obj.read_obj();
  cout << "obj['key2'] = "  << xr.get_string("key2") << endl 
       << "obj['float'] = " << xr.get_double("float") << endl 
       << "obj['int'] = "   << xr.get_int32("int") << endl 
       << "obj['large'] = " << xr.get_int64("large") << endl;

} // xb is destroyed here


int main(void) {
  test_c_api();
  test_cplusplus_api();
  return 0;
}
