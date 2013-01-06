#include <cassert>

#include "macros.h"
#include "xon/object.hh"
#include "object.c"

namespace xon {


/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/


CONSTRUCTOR_CPP
void init() 
{
  c_api::init();
}


DESTRUCTOR_CPP
void fini() 
{
  c_api::fini();
}


/*******************************************************
 *
 * Object
 *
 *******************************************************/

// obj::obj(c_api::obj o)
// {
// }

void obj::decref()
{
  std::cout << "decref " << ptr << " count=" << *refcount << std::endl;
  (*refcount)--;
  if (*refcount == 0) {
    std::cout << "decref free" << std::endl;
    free(ptr);
    delete refcount;
  }
}

void obj::refer(const obj& o)
{
  std::cout << "refer " << o.ptr << " count=" << *(o.refcount) << std::endl;
  refcount = o.refcount;
  ptr = o.ptr;
  (*refcount)++;
}


EXPORTED_SYMBOL_CPP
obj::obj(obj_builder& builder)
{
  refcount = new int(1);
  ptr = builder.get_c_api();
  std::cout << refcount << " " << ptr << std::endl;
}

EXPORTED_SYMBOL_CPP
obj::obj(const obj& o)
{
  refer(o);
}

EXPORTED_SYMBOL_CPP
obj::~obj()
{
  decref();
}

EXPORTED_SYMBOL_CPP
size_t obj::size() const
{
  return c_api::xon_obj_size(ptr);
}

EXPORTED_SYMBOL_CPP
const obj& obj::operator=(const obj& src)
{
  decref();
  refer(src);
  return *this;
}

EXPORTED_SYMBOL_CPP
std::ostream& operator << (std::ostream &out, const obj& object)
{
  char *output = c_api::xon_obj_string(object.ptr);
  if (output != NULL) {
    out << output << std::endl;
    free(output);
  }
  return out;
}



/*******************************************************
 *
 * Object builder
 *
 *******************************************************/


EXPORTED_SYMBOL_CPP
obj_builder::obj_builder()
{
  ptr = c_api::xon_obj_builder_new();
}

EXPORTED_SYMBOL_CPP
obj_builder::~obj_builder()
{
  c_api::xon_obj_builder_delete(ptr);
}

EXPORTED_SYMBOL_CPP
c_api::xon_obj obj_builder::get_c_api()
{
  return c_api::xon_obj_builder_get(ptr);
}

EXPORTED_SYMBOL_CPP
obj_builder& obj_builder::add(std::string key, std::string value)
{
  c_api::xon_obj_builder_add_string(ptr, key.c_str(), value.c_str());
  return *this;
}

EXPORTED_SYMBOL_CPP
obj_builder& obj_builder::add(std::string key, double value)
{
  c_api::xon_obj_builder_add_double(ptr, key.c_str(), value);
  return *this;
}

EXPORTED_SYMBOL_CPP
obj_builder& obj_builder::add(std::string key, int32_t value)
{
  c_api::xon_obj_builder_add_int32(ptr, key.c_str(), value);
  return *this;
}

EXPORTED_SYMBOL_CPP
obj_builder& obj_builder::add(std::string key, int64_t value)
{
  c_api::xon_obj_builder_add_int64(ptr, key.c_str(), value);
  return *this;
}

EXPORTED_SYMBOL_CPP
obj obj_builder::get()
{
  return obj(*this);
}




} /* end namespace xon */
