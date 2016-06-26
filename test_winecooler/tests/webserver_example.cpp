#include <string>

#include "ZmqStream.h"
#include <Arduino.h>

#define byte uint8_t

class EthernetClient : public ZmqStream
{
public:
  EthernetClient() : ZmqStream() {
    reply.clear();
  }
  EthernetClient(const EthernetClient &other) : ZmqStream(other ){
    reply = other.reply;
  }
  bool connected(void){return true;}
  void print(const std::string line) {reply.append(line);}
  void print(int val) {reply.append(std::to_string( val));}

  void println(const std::string line) {reply.append(line).append("\n");}
  void println() {reply.append("\n");}

  void stop(){
    write(reply);
    reply.clear();
    
  }
  virtual operator bool(){return this->available();}
private:
  std::string reply;
};


class EthernetServer 
{
public:
  EthernetServer(int port) {


  }
  void begin(void){

    responder = new zmq::socket_t(context, ZMQ_REP);
    printf("binding ");
    responder->bind("tcp://*:5559");
    printf("binded...\n");
    client.setSocket(responder);
        printf("socket set...\n");
    return;
  }
  
  EthernetClient available(void){
    return client;
  }
private:
  EthernetClient client;
  zmq::socket_t * responder;
  zmq::context_t context;

};

class IPAddress
{
public:
  IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    _a = a; _b = b; _c = c; _d = d;
  }
  
private:
  uint8_t _a, _b, _c, _d;
};

class MockEthernet {
public:
  void begin(byte mac[6], IPAddress ip){}
  const std::string localIP(){ return "";}
};
MockEthernet Ethernet;


#include "webserver_example.ino"

int main()
{
  
 
  
  setup();
  
  for(;;){
    loop();  
  }
  
}
