#ifndef _HANDLERH_
#define _HANDLERH_

#include <Arduino.h>

class AbstractHandler;

class ApiRequest;

class HandlerList
{
friend class AbstractHandler;
public:
  HandlerList();
  HandlerList * next();
  bool hasNext();
  void link(HandlerList * item);
  AbstractHandler * handler;
  
private:
  HandlerList * nextItem;
};


class AbstractHandler
{
public:
  AbstractHandler();
  const char * getLocation(void);
  void setLocation(const char * location);
  HandlerList * getLink();
  virtual bool canHandle(const char * location){
    return false;
  }
  virtual bool handle(ApiRequest * request){
    return false;
  }
  
protected:
  const char * location;
  HandlerList link;
};


#endif

