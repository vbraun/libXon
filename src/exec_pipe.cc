
#include "xon/object.hh"
#include "xon/server.hh"
#include "subprocess.hh"
#include "debug.h"




int main(void)
{
  using namespace std;

  xon::server server;
  xon::obj_builder xb;

  while (true) {
    const xon::object input = server.receive();
    const xon::obj_reader xr(input);
  
    if (xr.get_bool("quit", false))
      break;

    const string command = xr.get_string("command");
    vector<string> args; // todo
    xon::subprocess cmd = xon::subprocess(command);
    
    cmd << xr.get_string("stdin");
    xb.add("stdout", cmd.stdout());
    xb.add("stderr", cmd.stderr());
    xb.add("status", cmd.exit_status());
    const xon::object output(xb);

    server.send(output);
  }

  return 0;
}
