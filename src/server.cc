#include <cassert>

#include "macros.h"

#include "xon/support.h"
#include "xon/status.h"
#include "xon/object.hh"
#include "xon/server.hh"
#include "xon/exceptions.hh"
#include "server.c"


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
 * Server
 *
 *******************************************************/



EXPORTED_SYMBOL_CPP
server::server()
{
  ptr = c_api::xon_server_new();
  if (ptr == NULL)
    throw connect_exception("server: could not establish connection.");
}

EXPORTED_SYMBOL_CPP
server::~server()
{
  c_api::xon_server_delete(ptr);
}

EXPORTED_SYMBOL_CPP
object server::receive() const
{
  using namespace c_api;
  xon_obj c_obj;
  xon_status rc = 
    xon_server_receive(ptr, (xon_obj*)&c_obj);
  if (rc != XON_OK)
    throw receive_exception("server: error receiving.");
  return object(c_obj);
}

EXPORTED_SYMBOL_CPP
void server::send(const object& obj) const
{
  using namespace c_api;
  xon_status rc = 
    xon_server_send(ptr, (xon_obj)obj.pointer());
  if (rc != XON_OK)
    throw send_exception("server: error sending.");
}





} /* end namespace xon */
