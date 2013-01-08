#ifndef XON__OBJECT__HH
#define XON__OBJECT__HH

/** @file */ 

#include <inttypes.h>
#include <iostream>
#include <string>

#define XON_WITH_TR1 1
#if XON_WITH_TR1
#include <tr1/memory>
#endif 

#include <xon/object.h>


namespace xon {

class builder;
class reader;
class obj_reader;

#if XON_WITH_TR1
typedef std::tr1::shared_ptr<reader> reader_ptr;
#endif

//! Reference-counted immutable memory block
/*! The obj class implements an immutable reference-counted memory
 *  block holding a binary object.
 */
class object
{
private:
  int *refcount;
  c_api::xon_obj ptr;
  void decref();
  void refer(const object& o);
protected:
public:
  //! Construct reference to the given memory block
  /*  @param data memory block. Will be freed in the destructor.
   */
  object(const c_api::xon_obj c_obj);
  //! Construct object from key/value pairs in builder
  object(builder& b);
  //! Reference same memory block as o
  object(const object& o);
  ~object();
  //! Make this object a reference to src
  const object& operator=(const object& src);
  //! Return the size in bytes
  size_t size() const;
  //! Return whether this object is BSON
  bool is_bson() const;
  //! Return whether this object can be read
  /*  True if the object is either architecture independent or if it
   *  matches the host architecture.
   */ 
  bool is_host_arch() const;
#if XON_WITH_TR1
  //! Return a reader object appropriate for the object
  const reader_ptr read() const;
#endif
  //! Return the reader for binary objects
  const obj_reader read_obj() const;
  //! Return a pointer to the memory block
  const void* pointer() const;
  //! Pretty print to string
  std::string string() const;
  //! Debug output
  void hexdump() const;
  //! Pretty print
  friend std::ostream& operator << (std::ostream &out, const object& o);
};



//! Abstract base class for builders
class builder
{
private:
protected:
  friend class object;
  virtual c_api::xon_obj get_c_api() = 0;
public:
  virtual builder& add(std::string key, const char* value) = 0;
  virtual builder& add(std::string key, std::string value);
  virtual builder& add(std::string key, double value) = 0;
  virtual builder& add(std::string key, int32_t value) = 0;
  virtual builder& add(std::string key, int64_t value) = 0;
  virtual builder& add(std::string key, bool value) = 0;
  object get();
};


//! Builder for binary objects
class obj_builder : public builder
{
private:
  c_api::xon_obj_builder ptr;
protected:
  c_api::xon_obj get_c_api();
public:
  obj_builder();
  obj_builder(int) {};
  virtual ~obj_builder();
  using builder::add;  // access add(std::string, std::string)
  virtual obj_builder& add(std::string key, const char* value);
  virtual obj_builder& add(std::string key, double value);
  virtual obj_builder& add(std::string key, int32_t value);
  virtual obj_builder& add(std::string key, int64_t value);
  virtual obj_builder& add(std::string key, bool value);
};





//! Abstract base class for readers
class reader
{
private:
protected:
  const object obj;
public:
  reader(const object& o);
  virtual bool has_key(std::string key) const = 0;
  virtual std::string get_string(std::string key) const = 0;
  virtual std::string get_string(int pos) const = 0;
  virtual double      get_double(std::string key) const = 0;
  virtual double      get_double(int pos) const = 0;
  virtual int32_t     get_int32 (std::string key) const = 0;
  virtual int32_t     get_int32 (int pos) const = 0;
  virtual int64_t     get_int64 (std::string key) const = 0;
  virtual int64_t     get_int64 (int pos) const = 0;
  virtual bool        get_bool  (std::string key) const = 0;
  virtual bool        get_bool  (int pos) const = 0;
};


//! Reader for binary objects
class obj_reader : public reader
{
private:
  c_api::xon_obj_reader ptr;
public:
  obj_reader(const object& o);
  virtual ~obj_reader();
  virtual bool has_key(std::string key) const;
  virtual std::string get_string(std::string key) const;
  virtual std::string get_string(int pos) const;
  virtual double      get_double(std::string key) const;
  virtual double      get_double(int pos) const;
  virtual int32_t     get_int32 (std::string key) const;
  virtual int32_t     get_int32 (int pos) const;
  virtual int64_t     get_int64 (std::string key) const;
  virtual int64_t     get_int64 (int pos) const;
  virtual bool        get_bool  (std::string key) const;
  virtual bool        get_bool  (int pos) const;
};



} /* end namespace xon */

#endif /* XON__OBJECT__HH */
