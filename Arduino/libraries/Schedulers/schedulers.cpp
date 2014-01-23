#include "schedulers.h"

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

Schedulers::Schedulers()
{
  head = NULL;
}

void Schedulers::attach(TriggerAble * sched)
{
  sched->next = head;
  head = sched;
}

void Schedulers::attach(TriggerAble& sched)
{
  attach(&sched);
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
void Schedulers::remove(TriggerAble& sched)
{
  remove(&sched);
}
