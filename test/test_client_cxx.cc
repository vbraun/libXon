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

  xon::client client_c(command);
  client_c.send(obj);
  
  xon::object result_c = client_c.receive();
  cout << "Response from C client:" << endl << result_c << endl;
}




int main(void)
{
  test("./test_server_c");
  test("./test_server_cxx");
  return 0;
}
