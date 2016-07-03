#include <Ethernet.h> 

#include "../handler/handler.h"
#define KEY_BUFFER_SIZE 16


class ApiServer;

class LocationConfig
{
friend class ApiServer;

public:
  void use(AbstractHandler * handler);
  
  const char * location;
private:
  ApiServer * server;
};

class ApiServer
{
friend class LocationConfig;
public:
  ApiServer();
  ApiServer(EthernetServer &http){
    server = http;
  };
  void begin(void){
    server.begin();
  }
  LocationConfig * on(const char * location);
  void handleIncommingRequests();

protected:
  HandlerList * handlers;

private:
  bool delegate(ApiRequest * request);
  LocationConfig config;
  EthernetServer server = EthernetServer(80);
  
public:
  
};

enum methods {GET, PUT, POST, NONE};

enum requestState {UNKNOWN, VALID, INVALID};
class ApiRequest
{
public:
  ApiRequest();
  void setStream(Stream * stream){ this->stream = stream;}
  bool valid();
  void sendJsonHeaders();

  void respond(char * body);
  void respond(int body);
  
  requestState initialize();
  methods method();
  int getInstanceId();
  int intData();
  char * getKey();
private:
  Stream * stream;
  methods _method;
  requestState state;
  int _id;
  char _key[KEY_BUFFER_SIZE];
  int _value;

};