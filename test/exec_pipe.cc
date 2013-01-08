#include <iostream>

#include <xon/client.hh>

using std::cout;
using std::endl;





int main(void)
{
  using namespace std;

  xon::obj_builder xb;
  xb.add("command", "bc");
  xb.add("stdin", "2^3^4\n");
  
  xon::object input(xb);
  cout << "Sending for evaluation:" << endl << input << endl;

  xon::client client("../src/xon_server_exec_pipe");
  client.send(input);
  
  xon::object output = client.receive();
  cout << "Received result:" << endl << output << endl;

  xb.add("quit", true);
  xon::object poison_pill(xb);
  client.send(poison_pill);
  client.wait(5);

  return 0;
}
