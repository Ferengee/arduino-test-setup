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

enum methods {GET, PUT, NONE};

class ApiRequest
{
public:
  ApiRequest();
  void setStream(Stream * stream){ this->stream = stream;}
  void initialize(){}
  methods method();
  int intData(int *data){ return -1;}
  char * getPath(){return NULL;}
private:
  Stream * stream;
  methods _method;
};