#include "../handler/handler.h"

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
  LocationConfig * on(const char * location);
  void handle(const char * location);

protected:
  HandlerList * handlers;

private:
  LocationConfig config;
  
public:
  
};

enum methods {GET, PUT, POST, NONE};

class ApiRequest
{
public:
  ApiRequest();
  void setStream(Stream * stream){ this->stream = stream;}
  void initialize();
  methods method();
  int getInstanceId();
  int intData();
  char * getKey();
private:
  Stream * stream;
  methods _method;
  int _id;
  char _key[16];
  int _value;

};