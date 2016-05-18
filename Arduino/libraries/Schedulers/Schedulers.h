#ifndef Schedulers_h
#define Schedulers_h

#include <Arduino.h>

/*
 * Create a scheduler for a specific event
 * calling once or every reconfigures the scheduler
 * if an earlier call to once or every is not yet triggered
 * it will not be at all.
 * 
 * the trigger must be called by an external clock
 * the resolution of this clock determines the resolution
 * for this scheduler
 */
class Scheduler;
class Schedulers;

typedef void (* TimedEvent) (void * arguments);


class TriggerAble
{
  friend class Schedulers;
public:
  TriggerAble()
  { _arguments = NULL; _handler =  NULL;}
  virtual int trigger(){ return -1;}
  void init(TimedEvent handler, void * arguments)
    { _arguments = arguments; _handler = handler;}
protected:
  TriggerAble * next;
  TimedEvent _handler;
  void * _arguments;
};

class Scheduler : public TriggerAble
{
public:
  Scheduler();
  void once(unsigned long timeoutMillis, TimedEvent handler, void * arguments);
  void every(unsigned long timeoutMillis, TimedEvent handler, void * arguments);
  void stop();
  virtual int trigger();  
private:
  void configure(long unsigned int timeoutMillis, TimedEvent handler, void* arguments, bool onlyOnce);
  bool onlyOnce;
  unsigned long timeout;
  unsigned long lastexecution;
};

class CountdownTimer : public TriggerAble
{
public:
  CountdownTimer();
  void reset();
  void start(int start, TimedEvent handler, void * arguments);
  virtual int trigger();  

private:
  int from;
  int current;
};

class Schedulers
{
public:
  Schedulers();
  void attach(TriggerAble * sched);
  void remove(TriggerAble * sched);
  void attach(TriggerAble & sched);
  void remove(TriggerAble & sched);
  void trigger();
  
private:
  TriggerAble * head;
};

#endif