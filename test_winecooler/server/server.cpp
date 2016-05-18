#include "server.h"

void LocationConfig::use(AbstractHandler * handler)
{
  handler->getLink()->link(server->handlers);
  server->handlers = handler->getLink();
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
    item->handler->canHandle(location);
    item = item->next();
  }
  
}