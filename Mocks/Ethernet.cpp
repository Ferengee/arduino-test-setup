#include "Ethernet.h"
#include "ZmqContext.h"
void EthernetServer::begin(void){
  responder = new zmq::socket_t(ZmqStreamContext, ZMQ_REP);
  responder->bind("tcp://*:5559");
  client.setSocket(responder);
  return;
}