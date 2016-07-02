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

ApiServer::ApiServer() {
  config.server = this;
  handlers = NULL;
}
// TODO:
// handleRequests should create a ApiRequest
// and initialize it with the client
// close the client with client.stop() after a handler has succesfully handled the ApiRequest

void ApiServer::handle(const char * location)
{
  HandlerList * item = handlers;
  while(item != NULL){
    ((AbstractHandler *)item)->canHandle(location);
    item = item->next();
  }
  
}

void ApiServer::handleIncommingRequests()
{
  ApiRequest request;
  EthernetClient client = server.available();
  if(client){
    request.setStream(&client);
    if(request.valid()){
      delegate(&request);
    };
  }
}

ApiRequest::ApiRequest(){
  state = UNKNOWN;
  _method = NONE;
  stream = NULL;
  _id = -1;
  int i;
  _value = -1;

  for(i=0; i <  KEY_BUFFER_SIZE; i++)
    _key[i] = '\0';
  
}

methods ApiRequest::method()
{
  return _method;
}

bool ApiRequest::valid()
{
  if (state == UNKNOWN)
    state = initialize();
  return state == VALID;
}

requestState ApiRequest::initialize(){
  int len = 0;
  
  if(stream != NULL){
    len = stream->readBytesUntil('/', _key, KEY_BUFFER_SIZE);
    if (len == 0)
      return INVALID;
    _key[len] = '\0';
    
    
    if (strstr(_key, "PUT ") == _key)
      _method = PUT;
    
    else if (strstr(_key, "POST") == _key)
      _method = POST;

    else if (strstr(_key, "GET ") == _key)
      _method = GET;
    
    else 
      return INVALID;
    
    len = stream->readBytesUntil('/', _key, KEY_BUFFER_SIZE -1);
    _key[len] = '\0';
    if(strstr(_key, " HTTP") == _key)
      _key[0] = '\0';
    else
      _id = stream->parseInt();
    
    if (_method == POST){
      char lastchar = stream->read();
      char current = stream->read();
      while (!(lastchar == '\n' && current == '\r') && current > 0 && lastchar > 0 ){
        lastchar = current;
        current = stream->read();
      }
      current = stream->read();
      _value = stream->parseInt();
    }
    return VALID;
  }
  return INVALID;
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