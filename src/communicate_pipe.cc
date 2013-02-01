#include <cstdlib>
#include <iostream>

#include <xon/server.hh>
#include "subprocess.hh"
#include "debug.h"




xon::subprocess_factory configure(xon::object conf)
{

#ifdef __APPLE__
  factory.add_env("DYLD_INSERT_LIBRARIES", library);
#else
  factory.add_env("LD_PRELOAD", library);
#endif

  
}




int main(int argc, char *argv[])
{
  
  return 0;


  xon::server server;
  xon::obj_builder xb;

  xon::subprocess_factory factory = configure(server.receive());
  xon::subprocess_communicate cmd(factory);

  while (true) {
    const xon::object input = server.receive();
    const xon::obj_reader xr(input);
  
    if (xr.get_bool("quit", false))
      break;

    const string command = xr.get_string("command");
    vector<string> args; // todo
    xon::subprocess_factory factory(command);
    xon::subprocess_pipe cmd(factory);
    
    cmd << xr.get_string("stdin");
    xb.add("stdout", cmd.stdout());
    xb.add("stderr", cmd.stderr());
    xb.add("status", cmd.exit_status());
    const xon::object output(xb);

    server.send(output);
  }

  cmd.wait(1);
  return EXIT_SUCCESS;
}
