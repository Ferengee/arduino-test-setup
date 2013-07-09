#ifndef StreamParser_h
#define StreamParser_h
#include "BufferManager.h"
#include "StreamWrapper.h"
#include "StateMachine.h"

class StreamParser
{
public:
  int process();
  int test(){return nextState->_test;}
  StreamParser(){ 
    nextState = NULL;
    errorState = &error;
    endState = &end;
  }
protected:
  ReaderState * nextState;
  BufferManager * bufferManager;
  StreamWrapper * streamWrapper;
  ReaderState error;
  ReaderState end;
  ReaderState * errorState;
  ReaderState * endState;

};

class StringStreamParser : public StreamParser
{
public:
  StringStreamParser();
  void configure(const char startToken, const char endToken){ 
    reader.startToken = startToken;
    normal.endToken = endToken;
  }

  void setBufferManager(CharBufferManager * manager){bufferManager = manager;}
  void setStreamWrapper(StreamWrapper * wrapper){streamWrapper = wrapper;}
  void reset();

protected:

  ReaderStartState reader ;
  ReaderNormalState normal;
  ReaderEscapedState escaped;
  
};

class IntStreamParser : public StreamParser
{
public:
  IntStreamParser();

  void setBufferManager(IntBufferManager * manager){bufferManager = manager;}
  void setStreamWrapper(StreamWrapper * wrapper){streamWrapper = wrapper;}
  void reset();

protected:
  ReaderStartNumericState reader;

  ReaderSignedState sign;
  ReaderValueState value;
  ReaderFractionState fraction;
};

class FloatStreamParser : public IntStreamParser
{
public:
  FloatStreamParser();

  void setBufferManager(FloatBufferManager * manager){bufferManager = manager;}
  void setStreamWrapper(StreamWrapper * wrapper){streamWrapper = wrapper;}
};

#endif