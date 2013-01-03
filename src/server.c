#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "macros.h"
#include "xon/server.h"

NAMESPACE_XON_C_API_BEGIN

/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/


CONSTRUCTOR_C
void init() 
{
}


DESTRUCTOR_C
void fini() 
{
}


/*******************************************************
 *
 * Server
 *
 *******************************************************/

EXPORTED_SYMBOL_C
xon_server xon_server_new()
{
  xon_server server = (xon_server)
    malloc(sizeof(xon_server_struct));
  if (server == NULL)
    return NULL;
  return server;
}



EXPORTED_SYMBOL_C
void xon_server_delete(xon_server server)
{
  free(server);
}


EXPORTED_SYMBOL_C
xon_status xon_server_send(xon_server server, xon_obj obj)
{
  
  return XON_OK;
}


EXPORTED_SYMBOL_C
xon_status xon_server_receive(xon_server server, xon_obj *obj)
{
  return XON_OK;
}




NAMESPACE_XON_C_API_END
