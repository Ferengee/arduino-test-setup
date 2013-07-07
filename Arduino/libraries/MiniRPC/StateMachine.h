#ifndef StateMachine_h
#define StateMachine_h

#include "BufferManager.h"
#include <Arduino.h>
#include <StreamWrapper.h>


class ReaderState
{
public:
  ReaderState(){ _test=-1;}
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out){ return this;} 
  int _test;
  
};

class ReaderStartState : public ReaderState
{
public:
  ReaderStartState(){ 
    startToken = '"';
    errorState = NULL;
    normalState = NULL;
    _test = 1;
  }
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  
  ReaderState * errorState;
  ReaderState * normalState;
  int startToken;
  
};

class ReaderNormalState : public ReaderState
{
public:
  ReaderNormalState(){ 
    endToken = '"';
    errorState = NULL;
    escapedState = NULL;
    endState = NULL;
    _test = 2;
  }

  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  
  ReaderState * errorState;
  ReaderState * escapedState;
  ReaderState * endState;
  int endToken;
};

class ReaderEscapedState : public ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderEscapedState(){
    normalState = NULL;
    errorState = NULL;
    _test = 3;
  }
  ReaderState * normalState;
  ReaderState * errorState;

};
/*
 *  Float and int parser states
 */

class ReaderStartNumericState : public ReaderState
{
public:
  ReaderStartNumericState(){
    errorState = NULL;
    endState = NULL;
    signedState = NULL;
    fractionState = NULL;
    valueState = NULL;
    _test = 10;
  }
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
  ReaderSignedState(){
    errorState = NULL;
    fractionState = NULL;
    valueState = NULL;
    _test = 11;
  }
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderState * errorState;
  ReaderState * fractionState;
  ReaderState * valueState;
};

class ReaderValueState : public ReaderState
{
public:
  ReaderValueState(){
    endState = NULL;
    fractionState = NULL;
    _test = 12;
  }
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderState * endState;
  ReaderState * fractionState;
  
};

class ReaderFractionState : public ReaderState
{
public:
  ReaderFractionState(){
    endState = NULL;
    _test = 13;
    
  }
  virtual ReaderState * process(StreamWrapper * in, BufferManager * out);
  ReaderState * endState;
};



#endif