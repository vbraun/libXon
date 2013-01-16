#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include <iostream>

#include "macros.h"

#include "xon/status.h"
#include "xon/object.hh"
#include "xon/server.hh"
#include "xon/exceptions.hh"
#include "xon/preload.hh"

XON__EXTERN_C_BEGIN

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
int scanf(const char *format, ...)
{
  std::cout << "scanf " << format << std::endl;
  return 3;

  va_list args;
  va_start(args, format);
  int result = vscanf(format, args);
  va_end(args);
  return result;
}

EXPORTED_SYMBOL_CPP
int printf(const char *format, ...)
{
  std::cout << "printf " << format << std::endl;

  va_list args;
  va_start(args, format);
  int result = vprintf(format, args);
  va_end(args);
  return result;
}

  
ssize_t write(int fildes, const void *buf, size_t nbyte) 
{
  std::cout << "write " << nbyte << std::endl;

  if (nbyte == 0) return 0;
  GET_ORIGINAL(ssize_t, write, int, const void *, size_t);
  ssize_t n = ORIGINAL(write)(fildes, buf, nbyte);
  return n;
}


ssize_t read(int fildes, void *buf, size_t nbyte)
{
  std::cout << "read " << nbyte << std::endl;
  return 3;

  if (nbyte == 0) return 0;
  GET_ORIGINAL(ssize_t, read, int, const void *, size_t);
  ssize_t n = ORIGINAL(read)(fildes, buf, nbyte);
  return n;
}


ssize_t read_unlocked(int fildes, void *buf, size_t nbyte)
{
  std::cout << "read_unlocked " << nbyte << std::endl;
  return 3;

  if (nbyte == 0) return 0;
  GET_ORIGINAL(ssize_t, read_unlocked, int, const void *, size_t);
  ssize_t n = ORIGINAL(read_unlocked)(fildes, buf, nbyte);
  return n;
}




XON__EXTERN_C_END




namespace xon {


/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/


CONSTRUCTOR_CPP
void init() 
{
  std::cout << "preload init" << std::endl;
}


DESTRUCTOR_CPP
void fini() 
{
  std::cout << "preload fini" << std::endl;
}






namespace { 



} // end anonymous namespace




} // end namespace xon
