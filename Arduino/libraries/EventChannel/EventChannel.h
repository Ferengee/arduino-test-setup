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
  void addListener(Listener & listener);


private:
  Listener * listeners;
};


class Listener
{
  friend class EventChannel;

public:
  Listener();
  /*
   * returns false if the event is ignored
   * EventChannel does nothing with this information
   */
  virtual bool receive(int event, void * data){}
private:
  Listener * next;
};

#endif

