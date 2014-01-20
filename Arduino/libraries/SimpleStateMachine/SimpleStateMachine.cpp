#include "SimpleStateMachine.h"

void Vertex::join(State* start, State* end)
{
  this->startState = start;
  this->endState = end;
  start->link(this);
}

Vertex::Vertex(int token)
{
  this->token = token;
  this->nextSibbling = NULL;
  this->startState = NULL;
  this->endState = NULL;
}

void State::link(Vertex* outgoing)
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


State * Machine::getCurrentState()
{
  return this->currentState;
}
Machine::Machine(State * startState)
{
  this->currentState = startState;
}
bool Machine::receive(int token, void* data)
{
  State * nextState = this->currentState->tryToken(token);
  if(nextState != NULL){
    this->currentState = nextState;
    nextState->enterfunc(token, data);
  }
  return (nextState != NULL);
}
