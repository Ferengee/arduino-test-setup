#ifndef EventChannel_h
#define EventChannel_h

#include <Arduino.h>

class Listener;

class EventChannel
{
public:
  EventChannel();
  void send(int event, void * data);
  void addListener(Listener * listener);

private:
  Listener * listeners;
};


class Listener
{
  friend class EventChannel;

public:
  Listener();
  virtual void receive(int event, void * data){}
private:
  Listener * next;
};

#endif

