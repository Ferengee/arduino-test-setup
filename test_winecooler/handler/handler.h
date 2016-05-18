#ifndef _HANDLERH_
#define _HANDLERH_

#include <Arduino.h>

class AbstractHandler;

class ApiRequest;

class HandlerList
{
public:
  HandlerList();
  HandlerList * next();
  bool hasNext();
  void link(HandlerList * item);
  AbstractHandler * handler;
  
protected:
  HandlerList * nextItem;
};


class AbstractHandler : public HandlerList
{
public:
  AbstractHandler();
  const char * getLocation(void);
  void setLocation(const char * location);
  virtual bool canHandle(const char * location){
    return false;
  }
  virtual bool handle(ApiRequest * request){
    return false;
  }
  
protected:
  const char * location;
};


#endif

