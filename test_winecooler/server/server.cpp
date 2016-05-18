#include "server.h"

void LocationConfig::use(AbstractHandler * handler)
{
  handler->link(server->handlers);
  server->handlers = handler;
  handler->setLocation(location);
}

LocationConfig * ApiServer::on(const char * location)
{
  config.location = location;
  return &config;
}

ApiServer::ApiServer(){
  config.server = this;
  handlers = NULL;
}

void ApiServer::handle(const char * location)
{
  HandlerList * item = handlers;
  while(item != NULL){
    ((AbstractHandler *)item)->canHandle(location);
    item = item->next();
  }
  
}