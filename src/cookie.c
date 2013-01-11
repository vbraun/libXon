#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "xon/support.h"
#include "cookie.h"
#include "debug.h"

NAMESPACE_XON_C_API_BEGIN


const char* COOKIE_ENV_VAR = "XON_COOKIE";
const int COOKIE_LENGTH = 64;

const char *alphanumeric = 
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "0123456789";


char *new_cookie(int port)
{
  const int size = 1024;
  char *s = (char*)malloc(size);
  if (s == NULL)
    return s;
  snprintf(s, size, "%d:", port);
  char *pos = s + strlen(s);
  for (int i=0; i<COOKIE_LENGTH; i++) {
    int n = random() % strlen(alphanumeric);
    *pos++ = alphanumeric[n];
  }
  *pos = '\0';
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
}




NAMESPACE_XON_C_API_END
