#include <iostream>
#include <string>

#include <xon/object.hh>
#include <xon/client.hh>

using std::cout;
using std::endl;



void test(const std::string command)
{
  xon::obj_builder xb;
  xb.add("n", 39);
  xb.add("key", "value");
  
  xon::object obj = xb.get();
  cout << "Query:" << endl << obj << endl;

  xon::client client(command);
  client.send(obj);
  
  xon::object result = client.receive();
  cout << "Response from C client:" << endl << result << endl;
}




int main(void)
{
  test("./test_server_c");
  test("./test_server_cxx");
  return 0;
}
