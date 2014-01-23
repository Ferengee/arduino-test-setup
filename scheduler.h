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

typedef void (* TimedEvent) (void * arguments);

class Scheduler
{
public:
  Scheduler();
  void once(unsigned long timeoutMillis, TimedEvent handler, void * arguments);
  void every(unsigned long timeoutMillis, TimedEvent handler, void * arguments);
  void stop();
  void trigger();
  Scheduler * next;
  
private:
  bool onlyOnce;
  unsigned long timeout;
  unsigned long lastexecution;
  TimedEvent handler;
  void * arguments;
  
};

class CountdownTimer
{
public:
  CountdownTimer();
  int trigger();
  void reset();
  void start(int start, TimedEvent handler, void * arguments);
private:
  TimedEvent handler;
  int from;
  int current;
  void * arguments;
};


CountdownTimer::CountdownTimer()
{
  this->from = -1;
  this->current = -1;
  this->handler = NULL;
  this->arguments = NULL;
}

void CountdownTimer::start(int start, TimedEvent handler, void* arguments)
{
  this->from = start;
  this->current = start;
  this->handler = handler;
  this->arguments = arguments;
}


int CountdownTimer::trigger()
{
  if(current > -1)
    current--;
  if (current == 0 && this->handler != NULL)
    handler(this->arguments);
  return current;
}

void CountdownTimer::reset()
{
  this->current = this->from;
}


Scheduler::Scheduler()
{
  onlyOnce = true;
  handler = NULL;
  next = NULL;
  arguments = NULL;
}

void Scheduler::every(long unsigned int timeoutMillis, TimedEvent handler, void* arguments)
{
  this->once (timeoutMillis, handler, arguments);
  this->onlyOnce = false;
}

void Scheduler::once(long unsigned int timeoutMillis, TimedEvent handler, void* arguments)
{
  this->handler = handler;
  this->timeout = timeoutMillis;
  this->onlyOnce = true;
  this->lastexecution = millis();
  this->arguments = arguments;
}

void Scheduler::stop()
{
  this->handler =  NULL;
}

void Scheduler::trigger()
{
  if (this->handler == NULL)
    return;
  
  unsigned long now = millis();
  if((this->lastexecution + this->timeout) < now){
    this->lastexecution = now;
    this->handler(this->arguments);
  }
}

class Schedulers
{
public:
  Schedulers();
  void attach(Scheduler * sched);
  void remove(Scheduler * sched);
  void trigger();
  
private:
  Scheduler * head;
};

Schedulers::Schedulers()
{
  head = NULL;
}

void Schedulers::attach(Scheduler* sched)
{
  sched->next = head;
  head = sched;
}
void Schedulers::trigger()
{
  Scheduler * current = head;
  while(current != NULL){
    current->trigger();
    current = current->next;
  }
}

void Schedulers::remove(Scheduler* sched)
{
  if (sched == head || head == NULL){
    head = NULL;
    return;
  }
  
  Scheduler * last = head;
  Scheduler * current = head->next;

  while(current != NULL){
    if(current == sched){
        last->next = sched->next;
        break;
    }
  }
}
