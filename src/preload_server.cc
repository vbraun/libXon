#include <iostream>
#include "subprocess.hh"




int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: preload_server <command> [arguments]" << std::endl;
    return 1;
  }

  xon::subprocess_factory factory(argv[1]);
  for (char **arg = argv + 2; *arg != NULL; arg++)
    factory.add_arg(*arg);
  
#ifdef __APPLE__
  factory.add_env("DYLD_INSERT_LIBRARIES", "libxon-preload.so");
#else
  factory.add_env("LD_PRELOAD", "libxon-preload.so");
#endif

  xon::subprocess(factory).wait(-1);
  return 0;
}
