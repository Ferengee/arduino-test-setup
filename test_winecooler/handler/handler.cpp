#include "handler.h"

AbstractHandler::AbstractHandler(){
  location = NULL;
  link.handler = this;
}

void AbstractHandler::setLocation(const char * loc){
  location = loc;
}

const char * AbstractHandler::getLocation(void){
  return location;
}

HandlerList * AbstractHandler::getLink(void){
  return &link;
}

HandlerList::HandlerList(){
  nextItem = NULL;
  handler = NULL;
}


void HandlerList::link(HandlerList * item){
  nextItem = item;
}


HandlerList * HandlerList::next(void){
  return nextItem;
}

bool HandlerList::hasNext(void){
  return nextItem != NULL;
}