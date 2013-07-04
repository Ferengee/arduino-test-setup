#ifndef StreamParser_h
#define StreamParser_h
#include "../../../test_string_reader/BufferManager.h"
#include "StreamWrapper.h"
#include "StateMachine.h"

class StreamParser
{
public:
  int process();
  StreamParser(){}
  
protected:
  ReaderState * nextState;
  BufferManager * bufferManager;
  StreamWrapper * streamWrapper;
  ReaderState error;
  ReaderState endstate;

};

class StringStreamParser : public StreamParser
{
public:
  StringStreamParser();
  void setBufferManager(CharBufferManager * manager){bufferManager = manager;}
  void setStreamWrapper(StreamWrapper * wrapper){streamWrapper = wrapper;}

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