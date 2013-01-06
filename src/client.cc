#include <cassert>


#include "macros.h"

#include "xon/support.h"
#include "xon/status.h"

#include "xon/object.hh"
#include "xon/client.hh"
#include "xon/exceptions.hh"
#include "client.c"


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
 * Client
 *
 *******************************************************/

EXPORTED_SYMBOL_CPP
client::client(const std::string& command)
{
  ptr = c_api::xon_client_new(command.c_str());
  if (ptr == NULL)
    throw connect_exception("client: could not establish connection.");
}


EXPORTED_SYMBOL_CPP
client::~client()
{
  c_api::xon_client_delete(ptr);
}

EXPORTED_SYMBOL_CPP
void client::send(const object& obj) const
{
  using namespace c_api;
  xon_status rc = 
    xon_client_send(ptr, (xon_obj)obj.pointer());
  if (rc != XON_OK)
    throw send_exception("client: error sending.");
}

EXPORTED_SYMBOL_CPP
object client::receive() const
{
  using namespace c_api;
  xon_obj c_obj;
  xon_status rc = 
    xon_client_receive(ptr, (xon_obj*)&c_obj);
  if (rc != XON_OK)
    throw receive_exception("client: error receiving.");
  return object(c_obj);
}

EXPORTED_SYMBOL_CPP
void client::wait(double timeout) const
{
  using namespace c_api;
  xon_status rc = xon_client_wait(ptr, timeout);
  if (rc != XON_OK)
    throw send_exception("client: error sending.");
}

EXPORTED_SYMBOL_CPP
void client::kill() const
{
  c_api::xon_client_kill(ptr);
}



} /* end namespace xon */

