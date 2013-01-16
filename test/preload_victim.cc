#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>  // for getenv


/* Imagine this is your average closed-source interpreter. You don't
   have access to the source code so you can't change it and link
   against libXon. 
   
   EVIL HACK: we override C printf/scanf and C++ cin/cout. This works
   by overriding the functions / objects with with a shared library
   that we can force in via LD_PRELOAD.
   
*/



void test_printf_scanf()
{
  char buf[128];
  scanf("%10s", &buf);
  printf("input is >>>%s<<<", buf);
}


void test_read_write()
{
  char ch;
  read(STDIN_FILENO, &ch, 1);
  std::string reply = std::string("input is >>>") + ch + "<<<";
  write(STDOUT_FILENO, reply.c_str(), reply.size()+1);
}









int main(void)
{
  test_printf_scanf();
  test_read_write();
  return 0;
}
