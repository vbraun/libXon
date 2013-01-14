
#include "xon/object.hh"
#include "xon/server.hh"
#include "subprocess.hh"
#include "debug.h"




int test(std::string command, std::string input)
{
  using namespace std;
  xon::subprocess_factory factory(command);
  xon::subprocess_pipe cmd(factory);
  cmd << input;
  cmd.wait(1);
  cout << "Running " << command << endl;
  cout << "stdin:  " << input << endl;
  cout << "stdout: " << cmd.stdout() << endl;
  cout << "stderr: " << cmd.stderr() << endl;
  cout << "status: " << cmd.exit_status() << endl;
  return cmd.exit_status();
}





int main(int argc, const char *argv[])
{
  if (argc == 3)
    return test(argv[1], argv[2]);
  
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
    xon::subprocess_factory factory(command);
    xon::subprocess_pipe cmd(factory);
    
    cmd << xr.get_string("stdin");
    xb.add("stdout", cmd.stdout());
    xb.add("stderr", cmd.stderr());
    xb.add("status", cmd.exit_status());
    const xon::object output(xb);

    server.send(output);
  }

  return 0;
}
