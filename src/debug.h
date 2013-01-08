#ifndef XON__DEBUG__H
#define XON__DEBUG__H

/*! @file  */

#include "xon/support.h"


/*! Switch debugging on/off */
#define XON_DEBUG true

/*! Fill padding (empty space in xon_obj data) */
#define XON_FILL_PADDING true

#define debug_printf(...) \
  if (XON_DEBUG) fprintf(stderr, __VA_ARGS__);

#define error_printf(...) \
  fprintf(stderr, __VA_ARGS__);


#endif /* XON__DEBUG__H */
