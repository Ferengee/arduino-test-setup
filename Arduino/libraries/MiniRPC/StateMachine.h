#ifndef StateMachine_h
#define StateMachine_h

#include "BufferManager.h"
#include <Arduino.h>
#include <StreamWrapper.h>


class ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out){ return this;}  
};

class ReaderStartState : public ReaderState
{
public:
  ReaderStartState(){ startToken = '"';}
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  
  ReaderState * errorState;
  ReaderState * normalState;
  int startToken;
  
};

class ReaderNormalState : public ReaderState
{
public:
  ReaderNormalState(){ endToken = '"';}

  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  
  ReaderState * errorState;
  ReaderState * escapedState;
  ReaderState * endState;
  int endToken;
};

class ReaderEndState : public ReaderState
{

};

class ReaderEscapedState : public ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  
  ReaderState * normalState;
  ReaderState * errorState;

};
/*
 *  Float and int parser states
 */

class ReaderStartNumericState : public ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderState * errorState;
  ReaderState * endState;
  ReaderState * signedState;
  ReaderState * fractionState;
  ReaderState * valueState;
  
};

class ReaderSignedState : public ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderState * errorState;
  ReaderState * fractionState;
  ReaderState * valueState;
};

class ReaderValueState : public ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderState * endState;
  ReaderState * fractionState;
  
};

class ReaderFractionState : public ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderState * endState;
};



#endif