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

