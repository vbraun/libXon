
#include <xon/server.hh>


int64_t compute(const int64_t x)
{
  return x*x;
};



int main(void)
{
  xon::server server;
  xon::obj_builder builder;

  while (true) {
    const xon::object object = server.receive();
    const xon::obj_reader reader(object);
    
    if (reader.get_bool("quit", false))
      break;

    int64_t x = reader.get_int64("x");
    int64_t result = compute(x);
    builder.add("result", result);
    
    server.send(builder.get());
  }
  return 0;
}
