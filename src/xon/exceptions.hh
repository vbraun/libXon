#ifndef XON__EXCEPTIONS__HH
#define XON__EXCEPTIONS__HH

#include <stdexcept>

namespace xon {

//! Base class for all libXon exceptions
class exception : public std::runtime_error
{
public:
  exception(const std::string& what) : std::runtime_error(what) {};
};


//! The connection could not be established
class connect_exception : public exception
{
public:
  connect_exception(const std::string& what) : exception(what) {};
};


//! The object key is invalid
class key_exception : public exception
{
public:
  key_exception(const std::string& what) : exception(what) {};
};


//! Error during sending something
class send_exception : public exception
{
public:
  send_exception(const std::string& what) : exception(what) {};
};


//! Error during receiving something
class receive_exception : public exception
{
public:
  receive_exception(const std::string& what) : exception(what) {};
};


//! Timeout - perhaps not an error
class timeout_exception : public exception
{
public:
  timeout_exception(const std::string& what) : exception(what) {};
};




} /* end namespace xon */

#endif /* XON__EXCEPTIONS__HH */
