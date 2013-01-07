#ifndef XON__SERVER__HH
#define XON__SERVER__HH

/** @file */ 

#include <iostream>
#include <string>

#include <xon/server.h>
#include <xon/object.hh>
#include <xon/exceptions.hh>

namespace xon {



class server
{
private:
  c_api::xon_server ptr;
public:
  server();
  virtual ~server();
  object receive() const;
  void send(const object& obj) const;
};




} /* end namespace xon */

#endif /* XON__SERVER__HH */
