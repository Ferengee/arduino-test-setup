#include <BufferManager.h>
#include <DummySerial.h>

class StreamWrapper : public DummySerial
{
  
};

class ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, CharBufferManager * out){ return this;}
  bool done() {return _done;}

private:
  bool _done;
  
};

class ReaderStartState : public ReaderState
{
public:
  ReaderStartState(){ startToken = '"';}
  virtual ReaderState * process(StreamWrapper * in, CharBufferManager * out);
  
  ReaderState * errorState;
  ReaderState * normalState;
  int startToken;
  
};

class ReaderNormalState : public ReaderState
{
public:
  ReaderNormalState(){ endToken = '"';}

  virtual ReaderState * process(StreamWrapper * in, CharBufferManager * out);
  
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
  virtual ReaderState * process(StreamWrapper * in, CharBufferManager * out);
  
  ReaderState * normalState;
  ReaderState * errorState;

};
