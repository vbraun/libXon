#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "macros.h"
#include "xon/client.h"

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
 * Client
 *
 *******************************************************/


EXPORTED_SYMBOL_C
xon_client xon_client_new(const char *dst)
{
  xon_client client = (xon_client)
    malloc(sizeof(xon_client_struct));
  if (client == NULL)
    return NULL;

  return client;
}


EXPORTED_SYMBOL_C
void xon_client_delete(xon_client client)
{
  free(client);
}


EXPORTED_SYMBOL_C
xon_status xon_client_send(xon_client client, xon_obj obj)
{
  
  return XON_OK;
}


EXPORTED_SYMBOL_C
xon_status xon_client_receive(xon_client client, xon_obj *obj)
{
  return XON_OK;
}




NAMESPACE_XON_C_API_END
