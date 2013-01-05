#ifndef XON__DEBUG__H
#define XON__DEBUG__H

#include <stdbool.h>


#define XON_DEBUG true

#define debug_printf(...) \
  if (XON_DEBUG) fprintf(stderr, __VA_ARGS__);


#endif /* XON__DEBUG__H */
