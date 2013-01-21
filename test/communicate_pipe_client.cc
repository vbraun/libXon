#include <xon/client.hh>

using std::cout;
using std::endl;



int main(void)
{
  xon::obj_builder xb;
  std::vector<std::string> args;
  args.push_back("../src/.libs/libxon-stdbuf-preload.so");
  args.push_back("./communicate_pipe_victim");
  xon::client client("../src/xon_server_communicate_pipe", args);

  xb.add("key", "This is easy!");
  const xon::object input = xb.get();
  client.send(input);
  cout << "input:" << input;
  
  xon::object result = client.receive();
  xon::obj_reader xr(result);
  cout << "result:" << xr.get_string("result");

  if (xr.get_string("result") != std::string("I got your message: This is easy!"))
    return 1;

  client.wait(1);
  return 0;
}
