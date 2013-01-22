#include <xon/client.hh>

using std::cout;
using std::endl;



int main(void)
{
  xon::client client("../src/xon_server_communicate_pipe");

  xon::obj_builder xb;
  xb.add("prompt",     "Enter anything:");
  xb.add("ld_preload", "../src/.libs/libxon-stdbuf-preload.so");
  xb.add("program",    "./communicate_pipe_victim");
  client.send(xb.get());

  {
    xon::obj_reader xr(client.receive());
    cout << "banner (stdout):" << xr.get_string("stdout");
    cout << "banner (stderr):" << xr.get_string("stderr");
  }
  
  xon::obj_builder xb;
  xb.add("stdin", "test_123");
  xb.add("quit",  false);
  {
    const xon::object input = xb.get();
    client.send(input);
    cout << "input:" << input;
  }

  { 
    xon::obj_reader xr(client.receive());
    cout << "result (stdout):" << xr.get_string("stdout");
    cout << "result (stderr):" << xr.get_string("stderr");
  }

  //if (xr.get_string("result") != std::string("I got your message: This is easy!"))
  //  return 1;

  xb.add("stdin", "another_test");
  xb.add("quit",  false);
  {
    const xon::object input = xb.get();
    client.send(input);
    cout << "input:" << input;
  }

  { 
    xon::obj_reader xr(client.receive());
    cout << "result (stdout):" << xr.get_string("stdout");
    cout << "result (stderr):" << xr.get_string("stderr");
  }

  xb.add("quit",  true);
  client.send(xb.get());

  client.wait(1);
  return 0;
}
