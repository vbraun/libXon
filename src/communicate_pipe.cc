#include <iostream>
#include "subprocess.hh"




int main(int argc, char *argv[])
{
  if (argc < 3) {
    std::cerr << "Usage: preload_server <shared_library> <command> [arguments]" << std::endl;
    return 1;
  }
  std::string library(argv[1]);

  xon::subprocess_factory factory(argv[2]);
  for (char **arg = argv + 3; *arg != NULL; arg++)
    factory.add_arg(*arg);
  
#ifdef __APPLE__
  factory.add_env("DYLD_INSERT_LIBRARIES", library);
#else
  factory.add_env("LD_PRELOAD", library);
#endif

  xon::subprocess(factory).wait(-1);
  return 0;
}
