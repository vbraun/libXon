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

// object::obj(c_api::obj o)
// {
// }

void object::decref()
{
  std::cout << "decref " << ptr << " count=" << *refcount << std::endl;
  (*refcount)--;
  if (*refcount == 0) {
    std::cout << "decref free" << std::endl;
    free(ptr);
    delete refcount;
  }
}

void object::refer(const object& o)
{
  std::cout << "refer " << o.ptr << " count=" << *(o.refcount) << std::endl;
  refcount = o.refcount;
  ptr = o.ptr;
  (*refcount)++;
}

EXPORTED_SYMBOL_CPP
object::object(const c_api::xon_obj c_obj)
{
  ptr = c_obj;
  refcount = new int(1);
}


EXPORTED_SYMBOL_CPP
object::object(builder& b)
{
  refcount = new int(1);
  ptr = b.get_c_api();
  std::cout << refcount << " " << ptr << std::endl;
}

EXPORTED_SYMBOL_CPP
object::object(const object& o)
{
  refer(o);
}

EXPORTED_SYMBOL_CPP
object::~object()
{
  decref();
}

EXPORTED_SYMBOL_CPP
size_t object::size() const
{
  return c_api::xon_obj_size(ptr);
}

EXPORTED_SYMBOL_CPP
const object& object::operator=(const object& src)
{
  decref();
  refer(src);
  return *this;
}

EXPORTED_SYMBOL_CPP
const void* object::pointer() const
{
  return ptr;
}

EXPORTED_SYMBOL_CPP
bool object::is_bson() const
{
  return c_api::xon_obj_is_bson(ptr);
}

EXPORTED_SYMBOL_CPP
bool object::is_host_arch() const
{
  return c_api::xon_obj_verify_architecture(ptr);
}

#if XON_WITH_TR1
EXPORTED_SYMBOL_CPP
const reader_ptr object::read() const
{
  return reader_ptr(new obj_reader(*this));
}
#endif

EXPORTED_SYMBOL_CPP
const obj_reader object::read_obj() const
{
  return obj_reader(*this);
}


EXPORTED_SYMBOL_CPP
std::ostream& operator << (std::ostream &out, const object& object)
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
 * Object builder: base class
 *
 *******************************************************/


EXPORTED_SYMBOL_CPP
object builder::get()
{
  return object(get_c_api());
}


/*******************************************************
 *
 * Object builder: binary objects
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




/*******************************************************
 *
 * Object reader: abstract base class
 *
 *******************************************************/

reader::reader(const object& o)
  : obj(o)
{
}


/*******************************************************
 *
 * Object reader
 *
 *******************************************************/

EXPORTED_SYMBOL_CPP
obj_reader::obj_reader(const object& o)
  : reader(o)
{
  ptr = c_api::xon_obj_reader_new(const_cast<void*>(obj.pointer()));
}

EXPORTED_SYMBOL_CPP
obj_reader::~obj_reader()
{
  c_api::xon_obj_reader_delete(ptr);
}

EXPORTED_SYMBOL_CPP
std::string obj_reader::get_string(std::string key) const
{
  return std::string(c_api::xon_obj_reader_get_string_key(ptr, key.c_str()));
}

EXPORTED_SYMBOL_CPP
std::string obj_reader::get_string(int pos) const
{
  return std::string(c_api::xon_obj_reader_get_string_pos(ptr, pos));
}

EXPORTED_SYMBOL_CPP
double obj_reader::get_double(std::string key) const
{
  return c_api::xon_obj_reader_get_double_key(ptr, key.c_str());
}

EXPORTED_SYMBOL_CPP
double obj_reader::get_double(int pos) const
{
  return c_api::xon_obj_reader_get_double_pos(ptr, pos);
}

EXPORTED_SYMBOL_CPP
int32_t obj_reader::get_int32(std::string key) const
{
  return c_api::xon_obj_reader_get_int32_key(ptr, key.c_str());
}

EXPORTED_SYMBOL_CPP
int32_t obj_reader::get_int32(int pos) const
{
  return c_api::xon_obj_reader_get_int32_pos(ptr, pos);
}

EXPORTED_SYMBOL_CPP
int64_t obj_reader::get_int64(std::string key) const
{
  std::cout << "get_int64 key" << std::endl;

  return c_api::xon_obj_reader_get_int64_key(ptr, key.c_str());
}

EXPORTED_SYMBOL_CPP
int64_t obj_reader::get_int64(int pos) const
{  
  return c_api::xon_obj_reader_get_int64_pos(ptr, pos);
}



} /* end namespace xon */

