
#include <xon/server.hh>


xon::object make_obj()
{
  xon::obj_builder xb;
  xb.add("new",  "avalue");
  xb.add("new2", "avalue2");
  xb.add("new3", "abdcefgh");
  xb.add("number", 3.1415926535);
  xb.add("integer",   124);
  xb.add("long", (int64_t)(1) << 41);
  return xb.get();
}


int main(void)
{
  std::cout << "Server startup" << std::endl;
  xon::server server;

  std::cout << "Server: receiving message" << std::endl;
  const xon::object obj = server.receive();
  std::cout << "Server: received message" << std::endl;

  const xon::object reply = make_obj();
  server.send(reply);
  std::cout << "Server: sent reply" << std::endl;

  return 0;
}
