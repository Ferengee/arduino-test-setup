#include "Ethernet.h"
#include "ZmqContext.h"
void EthernetServer::begin(void){
  responder = new zmq::socket_t(ZmqStreamContext, ZMQ_REP);
  responder->bind("tcp://*:5559");
  client.setSocket(responder);
  return;
}


size_t EthernetClient::println()
{
  return reply.append("\r\n").length();
}

size_t EthernetClient::print(const std::string str)
{
  return reply.append(str).length();
}

size_t EthernetClient::print(int val)
{
  return reply.append(std::to_string(val)).length();
}
size_t EthernetClient::println(const std::string str)
{
  return reply.append(str).append("\r\n").length();
}

size_t EthernetClient::print(const char c_str[])
{
  std::string str(c_str, strlen(c_str));
  return print(str);
}
size_t EthernetClient::println(const char c_str[])
{
  std::string str(c_str, strlen(c_str));
  return println(str);
}