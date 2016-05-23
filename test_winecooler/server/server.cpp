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
  _id = -1;
  _key[0] = '\0';
  _value = -1;
}

methods ApiRequest::method()
{
 
  return _method;
}

void ApiRequest::initialize()
{

   if(stream != NULL){
    stream->readBytesUntil('/', _key, 5);
    _key[4] = '\0';
    
    if (strstr(_key, "PUT ") == _key)
      _method = PUT;
    
    if (strstr(_key, "POST") == _key)
      _method = POST;

    else if (strstr(_key, "GET ") == _key)
      _method = GET;
  
    int len = stream->readBytesUntil('/', _key, 15);
    _key[len] = '\0';
    _id = stream->parseInt();
    
    if (_method == POST){
      char lastchar = stream->read();
      char current = stream->read();
      while (!(lastchar == '\n' && current == '\n') && current > 0 && lastchar > 0 ){
        lastchar = current;
        current = stream->read();
      }
      _value = stream->parseInt();
    }
  }
}

int ApiRequest::getInstanceId(){
  
  return _id;
}

int ApiRequest::intData(){
  
  return _value;
}

char * ApiRequest::getKey(){
  
  return _key;
}