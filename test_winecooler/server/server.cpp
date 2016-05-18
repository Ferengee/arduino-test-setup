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

ApiRequest::ApiRequest(){
  _method = NONE;
  stream = NULL;
}

methods ApiRequest::method()
{
  if( _method == NONE && stream != NULL){
    char buf[5];
    stream->readBytesUntil('/', buf, 5);
    buf[3] = '\0';
    /*
    if (strstr(buf, "PUT ") == buf)
      _method = PUT;

    else if (strstr(buf, "GET ") == buf)
      _method = GET;
  
    */
  }
  return _method;
}