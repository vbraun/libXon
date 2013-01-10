#include <cassert>

#include "xon/server.hh"
#include "debug.h"

#include "mathlink_interface.hh"



void test(void)
{
  using namespace std;
  using namespace MmaMathLink;
  mathlink ml("/usr/local/Wolfram/Mathematica/9.0");
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
}

/////////////////////////////////////////////////////////
///
///  Main loop
///
////////////////////////////////////////////////////////


void run_server(void)
{
  using namespace std;
  using namespace MmaMathLink;
  xon::server server;
  mathlink ml();
  packet *pkt = NULL;
  pkt = ml.receive();
  
  delete pkt;
}


int main(void)
{
  test();
  // run_server();
  return 0;
}
