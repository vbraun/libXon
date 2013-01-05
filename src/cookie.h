#ifndef XON__COOKIE__H
#define XON__COOKIE__H


extern const char* COOKIE_ENV_VAR;

char *new_cookie(int port);
void parse_cookie(const char* cookie, int *port);


#endif /* XON__COOKIE__H */
