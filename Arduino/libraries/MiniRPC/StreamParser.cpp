#include "StreamParser.h"


StringStreamParser::StringStreamParser()
{
  reader = ReaderStartState();
  error = ReaderEndState();
  endstate = ReaderEndState();

  normal = ReaderNormalState();
  escaped = ReaderEscapedState();
  
  reader.errorState = &error;
  reader.normalState = &normal;
  reader.startToken = '"';
  
  normal.errorState = &error;
  normal.escapedState = &escaped;
  normal.endState = &endstate;
  normal.endToken = '"';
  
  escaped.normalState = &normal;
  escaped.errorState = &error;

  
  nextState = &reader;
}

int StreamParser::process()
{
    nextState = nextState->process(streamWrapper, bufferManager);
    if (nextState == &error)
      return -1;
    if (nextState == &endstate)
      return 1;
    return 0;
}

IntStreamParser::IntStreamParser()
{
  reader = ReaderStartNumericState();
  error = ReaderEndState();
  endstate = ReaderEndState();
  fraction = ReaderFractionState();
  sign = ReaderSignedState();
  value = ReaderValueState();
 
  reader.valueState = &value;
  reader.endState = &endstate;
  reader.fractionState = NULL;
  reader.signedState = &sign;
  reader.errorState = &error;
  
  value.fractionState = NULL;
  value.endState = &endstate;

  sign.fractionState = &fraction;
  sign.errorState = &error;
  sign.valueState = &value;

  fraction.endState = &endstate;
  
  nextState = &reader;
}

FloatStreamParser::FloatStreamParser()
{
  reader.fractionState = &fraction;
  value.fractionState = &fraction;
}