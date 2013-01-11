
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
    const string stdin = xr.get_string("stdin");
    vector<string> args; // todo

    stringstream stdout;
    stringstream stderr;
    int rc = xon::communicate(command, args, stdin, stdout, stderr);

    xb.add("stdout", stdout.str());
    xb.add("stderr", stderr.str());
    xb.add("exit", rc);
    const xon::object output(xb);

    server.send(output);
  }

  return 0;
}
