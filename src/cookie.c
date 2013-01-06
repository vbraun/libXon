#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cookie.h"
#include "debug.h"



const char* COOKIE_ENV_VAR = "XON_COOKIE";


char *new_cookie(int port)
{
  const int size = 1024;
  char *s = (char*)malloc(size);
  if (s == NULL)
    return s;
  snprintf(s, size, "%d", port);
  return s;
}


const char *get_cookie_from_environ(void)
{
  return getenv(COOKIE_ENV_VAR);
}



void parse_cookie(const char* cookie, int *port)
{
  const char *str = cookie;
  if (str == NULL)
    str = get_cookie_from_environ();
  if (str == NULL) {
    error_printf("Environment variable %s is missing.", COOKIE_ENV_VAR);
    return;
  }
  *port = atoi(str);
  debug_printf("parse_cookie: %s %d\n", str, *port);
}
