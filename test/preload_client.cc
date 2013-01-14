#include <xon/client.hh>

using std::cout;
using std::endl;



int main(void)
{
  xon::obj_builder xb;
  xon::client client("../src/xon_server_preload", "preload_victim");

  xb.add("key", "This is easy!");
  const xon::object input = xb.get();
  client.send(input);
  cout << "input:" << input;
  
  xon::object result = client.receive();
  xon::obj_reader xr(result);
  cout << "result:" << xr.get_string("result");

  client.wait(1);
  return 0;
}
