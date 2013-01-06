#include <iostream>

#include <xon/object.hh>
#include <xon/client.hh>





int main(void)
{
  xon::builder xb;
  xb.add("n", 39);
  xb.add("key", "value");
  
  xon::object obj = xb.get();
  cout << "Query:" << endl << obj << endl;

  // xon::client client_cc("file://localhost/test_server_c++");
  // client_cc.send(obj);

  // xon::obj result_cc = client_cc.receive();
  // cout << "Response from C++ client:" << endl << result_cc << endl;

  xon::client client_c("file://localhost/test_server_c");
  client_c.send(obj);
  
  xon::obj result_c = client_c.receive();
  cout << "Response from C client:" << endl << result_c << endl;

  return 0;
}
