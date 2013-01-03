#ifndef XON_OBJECT__HH
#define XON_OBJECT__HH

/** @file */ 

#include <inttypes.h>
#include <iostream>
#include <string>

namespace xon {

namespace c_api {  /* xon::c_api */
#include <xon/object.h>
}


class obj;
class obj_builder;
//class reader;
//class obj_reader(reader);


//! Reference-counted memory block
/*! The obj class implements an immutable reference-counted memory
 *  block holding a binary object.
 */
class obj
{
  friend class obj_builder;
private:
  int *refcount;
  c_api::xon_obj ptr;
  void decref();
  void refer(const obj& o);
protected:
public:
  obj(obj_builder& builder);
  obj(const obj& o);
  ~obj();
  size_t size() const;
  const obj& operator=(const obj& src);
  friend std::ostream& operator << (std::ostream &out, const obj& o);
};



class obj_builder
{
  friend class obj;
private:
  c_api::xon_obj_builder ptr;
protected:
  c_api::xon_obj get_c_api();
public:
  obj_builder();
  obj_builder(int) {};
  ~obj_builder();
  obj_builder& add(std::string key, std::string value);
  obj_builder& add(std::string key, double value);
  obj_builder& add(std::string key, int32_t value);
  obj_builder& add(std::string key, int64_t value);
  obj get();
};



} /* end namespace xon */


#endif /* XON_OBJECT__HH */
