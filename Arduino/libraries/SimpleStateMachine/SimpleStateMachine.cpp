#include "SimpleStateMachine.h"

void Vertex::join(State * start, State * end)
{
  this->startState = start;
  this->endState = end;
  start->link(this);
}
void Vertex::startFrom(State * startState, int token)
{
  this->startState = startState;
  this->token = token;
  startState->link(this);
}


Vertex::Vertex()
{
  Vertex(0);
}

Vertex::Vertex(int token)
{
  this->token = token;
  this->nextSibbling = NULL;
  this->startState = NULL;
  this->endState = NULL;
}

Vertex* State::on(Vertex & via, int token)
{
  return this->on(&via, token);
}


Vertex * State::on(Vertex * via, int token)
{
  via->startFrom(this, token);
  return via;
}


void State::link(Vertex * outgoing)
{
  outgoing->nextSibbling = this->outgoing;
  this->outgoing = outgoing;
}

State * State::tryToken(int token)
{
  Vertex * vertex = this->outgoing;
  while(vertex != NULL)
  {
    if(vertex->accept(token))
      return vertex->endState;
    
    vertex = vertex->nextSibbling;
  }
  
  return NULL;
}

State::State()
{
  this->outgoing = NULL;
}

void Machine::start()
{
  start(NULL);
}

void Machine::start(void* data)
{
  this->started = true;
  enter(this->currentState, 0, data);
}


void Machine::enter(State * nextState, int token, void * data)
{
  this->currentState = nextState;
  if(nextState->enterfunc != NULL)
    nextState->enterfunc(token, data);
}

State * Machine::getCurrentState()
{
  return this->currentState;
}
Machine::Machine(State * startState)
{
  this->currentState = startState;
}
Machine::Machine(State & startState)
{
  this->currentState = &startState;
}
bool Machine::receive(int token, void * data)
{
  if(!started)
    return false;
  State * nextState = this->currentState->tryToken(token);
  bool accepted = nextState != NULL;
  if(accepted)
    enter(nextState, token ,data);
  return accepted;
}
