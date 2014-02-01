#include <Arduino.h>
#include <EventChannel.h>

typedef void  (* EnterFunc) (int token, void * data);

class State;

class Vertex
{
private:
  int token;

public:
  Vertex(int token);
  Vertex();
  void join(State * start, State * end);
  bool accept(int token){
    return this->token == token;
  }
  Vertex * nextSibbling;
  State * startState;
  void to(State * endState){ this->endState = endState;}
  void to(State & endState){ this->endState = &endState;}

  void startFrom(State * startState, int token);
  State * endState;

};

class State
{
public:
  State();
  EnterFunc enterfunc;
  Vertex * outgoing;

  void link(Vertex * outgoing);
  State * tryToken(int token);
  Vertex * on(Vertex * via, int token);
  Vertex * on(Vertex & via, int token);
};

class Machine : public Listener
{
private:
  State * currentState;
  void enter(State * state, int token, void * data);
public:
  Machine(State * startState);
  Machine(State & startState);
  State * getCurrentState();
  void start();
  void start(void * data);
  virtual bool receive(int token, void * data);
};
