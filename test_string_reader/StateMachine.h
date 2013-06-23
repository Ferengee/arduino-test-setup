#include <buffer_manager.h>
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
  
  virtual ReaderState * process(StreamWrapper * in, CharBufferManager * out);
  
  ReaderState * errorState;
  ReaderState * normalState;
  
};

class ReaderNormalState : public ReaderState
{
public:
  virtual ReaderState * process(StreamWrapper * in, CharBufferManager * out);
  
  ReaderState * errorState;
  ReaderState * escapedState;
  ReaderState * endState;
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
