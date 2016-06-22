#include <Arduino.h>
#define byte uint8_t

class EthernetClient {
public:
  bool connected(void){return true;}
  bool available(void){return true;}
  int read(void){return '\n';}
  void print(const std::string line) {cout << line;}
  void print(int val) {printf("%d", val);}

  void println(const std::string line) {cout << line << "\n";}
  void println() {cout << "\n";}

  void stop(){}
  virtual operator bool(){return this->available();}
};


class EthernetServer 
{
public:
  EthernetServer(int port) {}
  void begin(void){}
  EthernetClient available(void){
    return client;
  }
private:
  EthernetClient client;
  
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
