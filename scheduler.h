#include <sys/time.h>


unsigned long millis(void){
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


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


CountdownTimer::CountdownTimer()
{
  this->from = -1;
  this->current = -1;

}

void CountdownTimer::start(int start, TimedEvent handler, void* arguments)
{
  this->from = start;
  this->current = start;
  this->init(handler, arguments);
}


int CountdownTimer::trigger()
{
  if(current > -1)
    current--;
  if (current == 0 && _handler != NULL)
    _handler(_arguments);
  return current;
}

void CountdownTimer::reset()
{
  this->current = this->from;
}


Scheduler::Scheduler()
{
  onlyOnce = true;
}

void Scheduler::every(long unsigned int timeoutMillis, TimedEvent handler, void* arguments)
{
  this->once (timeoutMillis, handler, arguments);
  this->onlyOnce = false;
}

void Scheduler::once(long unsigned int timeoutMillis, TimedEvent handler, void* arguments)
{
  this->init(handler, arguments);
  this->timeout = timeoutMillis;
  this->onlyOnce = true;
  this->lastexecution = millis();
}

void Scheduler::stop()
{
  _handler =  NULL;
}

int Scheduler::trigger()
{
  if (_handler == NULL)
    return -1;
  
  unsigned long now = millis();
  if((this->lastexecution + this->timeout) < now){
    this->lastexecution = now;
    _handler(_arguments);
    return 1;
  }
  return 0;
}

class Schedulers
{
public:
  Schedulers();
  void attach(TriggerAble * sched);
  void remove(TriggerAble * sched);
  void trigger();
  
private:
  TriggerAble * head;
};

Schedulers::Schedulers()
{
  head = NULL;
}

void Schedulers::attach(TriggerAble * sched)
{
  sched->next = head;
  head = sched;
}
void Schedulers::trigger()
{
  TriggerAble * current = head;
  while(current != NULL){
    current->trigger();
    current = current->next;
  }
}

void Schedulers::remove(TriggerAble* sched)
{
  if (sched == head || head == NULL){
    head = NULL;
    return;
  }
  
  TriggerAble * last = head;
  TriggerAble * current = head->next;

  while(current != NULL){
    if(current == sched){
        last->next = sched->next;
        break;
    }
  }
}
