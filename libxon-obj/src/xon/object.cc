#include <cassert>

#include "xon/object.hh"
#include "macros.h"


namespace xon::c_api {
#include "object.c"
}

/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/


CONSTRUCTOR_CPP
void init() 
{
}


DESTRUCTOR_CPP
void fini() 
{
}





