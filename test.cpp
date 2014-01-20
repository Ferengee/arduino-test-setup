#include <iostream>       
#include <stdlib.h>

#include <string.h>

using namespace std;


typedef void  (* EnterFunc) (int token, void * data);


class State;

class Vertex
{
private:
  int token;
public:
  Vertex(int token);
  void join(State * start, State * end);
  bool accept(int token){
    return this->token == token;
  }
  Vertex * nextSibbling;
  State * startState;
  State * endState;
  
};

class State
{
public:
  State();
  EnterFunc enterfunc;
  Vertex * outgoing;

  void link(Vertex * outgoing);
    State* tryToken(int token);
};


class Machine
{
private:
  State * currentState;
public:
  Machine(State * startState);
  State * getCurrentState();
  bool receive(int token, void * data);
};


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

void hello(int token, void * data){
  cout << "hello " << token << "\n";
}


void goodbye(int token, void * data){
  cout << "goodbye " << token << "\n";
}

void enterAState(int token, void *data){
  cout << "A state " << token << "\n";
}
void enterBState(int token, void *data){
    cout << "B state " << token << "\n";
}


int main(int argc, const char* argv[]){
  State endState;
  State startState;
  State aState;
  State bState;
  
  Vertex s1 = Vertex(1);
  Vertex s1_1 = Vertex(1);
  Vertex s2 = Vertex(2);
  Vertex s2_1 = Vertex(2);
  Vertex s3 = Vertex(3);
  Vertex s4 = Vertex(4);
  Vertex s5 = Vertex(1);

  s1.join(&startState, &aState);
  s1_1.join(&aState, &startState);
  s2.join(&startState, &bState);
  s2_1.join(&bState, &startState);
  s3.join(&aState, &endState);
  s4.join(&bState, &endState);
  s5.join(&endState, &startState);
  
  startState.enterfunc = hello;
  endState.enterfunc =goodbye;
  aState.enterfunc = enterAState;
  bState.enterfunc = enterBState;
  
  Machine machine = Machine(&startState);
  
  int steps[] = {1,1,2,3,4,3,4,1,2,4};
  int i;
  char data[] = "test data";
  for (i=0; i < 10; i++){
    bool succes = machine.receive(steps[i], (void *)data);
    if (!succes)
      cout << "skipped: " << steps[i] << "\n";
  }
}