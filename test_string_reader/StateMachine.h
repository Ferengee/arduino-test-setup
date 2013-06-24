#include "BufferManager.h"
#include <DummySerial.h>

/*
 * has the state machine to parse a specific format
 */
class StreamParser
{
  
};

/*
 * todo: implement wrapper as proxy
 * StreamWrapper HasA Stream, instead of IsA Stream
 * 
 * 
 */
class StreamWrapper : public DummySerial
{
};

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
