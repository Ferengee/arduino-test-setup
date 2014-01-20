#include <Arduino.h>

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
