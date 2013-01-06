#ifndef XON__COOKIE__H
#define XON__COOKIE__H

/** @file */ 

#include "xon/support.h"

NAMESPACE_XON_C_API_BEGIN



/*! @brief Name of the environment variable */
extern const char* COOKIE_ENV_VAR;


/*! @brief Create new cookie
 *
 *  @param port the port number to encode in the cookie
 *  @return a newly-allocated string. It is your responsibility to
 *          free it.
 */
char *new_cookie(int port);


/*! @brief Parse the cookie
 *
 *  @param cookie a cookie generated with new_cookie
 *  @param port the port number that is encoded in the cookie
 * 
 *  The cookie may be NULL, in which case it is read from the
 *  environment variable COOKIE_ENV_VAR.
 */
void parse_cookie(const char* cookie, int *port);


/*! @brief Get the cookie from the environment variable.
 *
 *  @return a string. You must not free it.
 */
const char *get_cookie_from_environ(void);




NAMESPACE_XON_C_API_END

#endif /* XON__COOKIE__H */
