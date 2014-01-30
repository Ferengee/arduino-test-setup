#include "EventChannel.h"

void EventChannel::addListener(Listener* listener)
{
  Listener * tail = listeners;
  listeners = listener;
  listener->next = tail;
}

EventChannel::EventChannel()
{
  listeners = NULL;
}

void EventChannel::send(int event, void* data)
{
  Listener * current = listeners;
  while (current != NULL)
  {
    current->receive(event, data);
    current = current->next;
  }
}

Listener::Listener()
{
  next = NULL;
}

