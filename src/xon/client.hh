#ifndef XON__CLIENT__HH
#define XON__CLIENT__HH

/** @file */ 

#include <iostream>
#include <string>

#include <xon/client.h>
#include <xon/object.hh>
#include <xon/exceptions.hh>

namespace xon {


class client_base
{
};


class client : public client_base
{
private:
  c_api::xon_client ptr;
public:
  client(const std::string& command);
  virtual ~client();
  void send(const object& obj) const;
  void wait(double timeout) const;
  void kill() const;
  object receive() const;
};




} /* end namespace xon */

#endif /* XON__OBJECT__HH */
