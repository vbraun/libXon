#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <iostream>

#include "macros.h"

#include "xon/status.h"
#include "xon/object.hh"
#include "xon/server.hh"
#include "xon/exceptions.hh"

#include "stdbuf_preload.hh"

XON__EXTERN_C_BEGIN

typedef int(*setvbuf_func)(FILE *stream, char *buf, int mode, size_t size);
static setvbuf_func original_setvbuf;





#define ORIGINAL(name) original_##name
#define GET_ORIGINAL(ret, name, ...)                         \
  static ret (*ORIGINAL(name))(__VA_ARGS__) = NULL;          \
  if (!ORIGINAL(name)) {                                     \
    *(void **)(&ORIGINAL(name)) = dlsym(RTLD_NEXT, #name);   \
  }                                                          \
  if (!ORIGINAL(name)) {                                     \
    errno = ENOSYS;                                          \
    abort();                                                 \
  }


EXPORTED_SYMBOL_CPP
int isatty(int fd)
{
  std::cout << "calling isatty(" << fd << ")" << std::endl;
  if (fd == STDIN_FILENO)   return 1;
  if (fd == STDOUT_FILENO)  return 1;
  if (fd == STDERR_FILENO)  return 1;
  GET_ORIGINAL(int, isatty, int);
  return ORIGINAL(isatty)(fd);
}




EXPORTED_SYMBOL_CPP
void setbuf(FILE *stream, char *buf)
{
  std::cout << "calling setbuf()" << std::endl;
  if      (stream == stdin)   original_setvbuf(stream, NULL, _IOLBF, 0);
  else if (stream == stdout)  original_setvbuf(stream, NULL, _IOLBF, 0);
  else if (stream == stderr)  original_setvbuf(stream, NULL, _IOLBF, 0);
  else 
    original_setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}

EXPORTED_SYMBOL_CPP
void setbuffer(FILE *stream, char *buf, size_t size)
{
  std::cout << "calling setbuffer()" << std::endl;
  if      (stream == stdin)   original_setvbuf(stream, NULL, _IOLBF, 0);
  else if (stream == stdout)  original_setvbuf(stream, NULL, _IOLBF, 0);
  else if (stream == stderr)  original_setvbuf(stream, NULL, _IOLBF, 0);
  else 
    original_setvbuf(stream, buf, buf ? _IOFBF : _IONBF, size);
}

EXPORTED_SYMBOL_CPP
void setlinebuf(FILE *stream)
{
  std::cout << "calling setlinebuf()" << std::endl;
  original_setvbuf(stream, NULL, _IOLBF, 0);
}

EXPORTED_SYMBOL_CPP
int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
  std::cout << "calling setvbuf()" << std::endl;
  if (stream == stdin)   return original_setvbuf(stream, NULL, _IOLBF, 0);
  if (stream == stdout)  return original_setvbuf(stream, NULL, _IOLBF, 0);
  if (stream == stderr)  return original_setvbuf(stream, NULL, _IOLBF, 0);
  return original_setvbuf(stream, buf, mode, size);
}





/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/


CONSTRUCTOR_CPP
void init() 
{
  fprintf(stderr, "preload init\n");
  original_setvbuf = (setvbuf_func)dlsym(RTLD_NEXT, "setvbuf");
  if (original_setvbuf == NULL) {
    std::cerr << "Dynamically linking setvbuf() failed: " 
              << strerror(errno) << std::endl;
    abort();
  }
  original_setvbuf(stdin,  NULL, _IOLBF, 0);
  original_setvbuf(stdout, NULL, _IOLBF, 0);
  original_setvbuf(stderr, NULL, _IOLBF, 0);
}


DESTRUCTOR_CPP
void fini() 
{
  fprintf(stderr, "preload fini\n");
}




XON__EXTERN_C_END
