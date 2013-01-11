#include <cassert>

#include "xon/server.hh"
#include "debug.h"

#include "mathlink_interface.hh"


int test_open_explicit(const char* mathematica_command, const char* mathlink_library)
{
  using namespace MmaMathLink;
  mathlink ml(mathematica_command, mathlink_library);
  return 0;
}


int test_mathlink(const char* mathematica_root_directory)
{
  using namespace std;
  using namespace MmaMathLink;
  mathlink ml(mathematica_root_directory);
  packet *pkt = NULL;

  pkt = ml.receive();
  assert(pkt->header() == packet::INPUT_NAME);
  cout << *pkt;

  ml.send(packet_function("EnterTextPacket", 1));
  ml.send(packet_string("1+1"));
  ml.send(packet_end());

  while (true) {
    delete pkt;
    pkt = ml.receive();
    cout << *pkt;
    if (pkt->header() == packet::INPUT_NAME)
      break;
  }

  ml.send(packet_function("EnterTextPacket", 1));
  ml.send(packet_string("N[Pi, 10000]"));
  ml.send(packet_end());

  while (true) {
    delete pkt;
    pkt = ml.receive();
    cout << *pkt;
    if (pkt->header() == packet::INPUT_NAME)
      break;
  }

  delete pkt;
  return 0;
}

/////////////////////////////////////////////////////////
///
///  Main loop
///
////////////////////////////////////////////////////////


std::string find_root_dir(const xon::object& obj)
{
  xon::obj_reader xr = xon::obj_reader(obj);
  if (xr.has_key("root_dir"))
    return xr.get_string("root_dir");
  std::string cmd = xr.get_string("command", "math");


  return cmd;
}


int run_communicate(xon::server& server, MmaMathLink::mathlink& ml)
{
  using namespace std;
  using namespace MmaMathLink;
  xon::obj_builder xb;

  packet *pkt = NULL;
  while (true) { // eat everything up to the first input prompt
    delete pkt;
    pkt = ml.receive();
    if (pkt->header() == packet::INPUT_NAME)
      break;
  }

  while (true) {
    xon::obj_reader xr = xon::obj_reader(server.receive());
    
    if (xr.get_bool("quit", false))
      break;

    string stdin = xr.get_string("stdin");
    ml.send(packet_function("EnterTextPacket", 1));
    ml.send(packet_string(stdin));
    ml.send(packet_end());

    string stdout;
    string prompt;
    while (true) {
      delete pkt;
      pkt = ml.receive();
      cout << *pkt;
      if (pkt->header() == packet::RETURN_TEXT)
        stdout += pkt->to_string();
      if (pkt->header() == packet::OUTPUT_NAME)
        prompt += pkt->to_string();
      else if (pkt->header() == packet::INPUT_NAME)
        break;  // input prompt
    }
    
    xb.add("stdout", stdout);
    xb.add("stderr", "");
    xb.add("prompt", prompt);
    server.send(xb.get());
  }
  
  delete pkt;
  return 0;
}


int run_server(void)
{
  using namespace MmaMathLink;
  xon::server server;
  xon::obj_builder xb;

  std::string root_dir = find_root_dir(server.receive());
  try {
    mathlink ml = mathlink(root_dir);
    xb.add("error", false);
    xb.add("version", ml.interface());
    xb.add("revision", ml.revision());
    xb.add("library", ml.shared_library());
    server.send(xb.get());
    return run_communicate(server, ml);
  } catch (mathlink_exception e) {
    xb.add("error", true);
    xb.add("message", e.what());
    server.send(xb.get());
    return 1;
  }
  return 2;
}    


int main(int argc, char *argv[])
{
  if (argc == 3)
    return test_open_explicit(argv[1], argv[2]);
  else if (argc == 2)
    return test_mathlink(argv[1]);
  else
    return run_server();
}
