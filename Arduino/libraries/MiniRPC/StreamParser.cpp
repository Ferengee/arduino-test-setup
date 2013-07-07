#include "StreamParser.h"


StringStreamParser::StringStreamParser()
{ 
  reader.errorState = &error;
  reader.normalState = &normal;
  reader.startToken = '"';
  
  normal.errorState = &error;
  normal.escapedState = &escaped;
  normal.endState = &end;
  normal.endToken = '"';
  
  escaped.normalState = &normal;
  escaped.errorState = &error;
  nextState = &reader;
}

void StringStreamParser::reset()
{
  nextState = &reader;
}


int StreamParser::process()
{     
  if(nextState == NULL)
    exit(1);
  nextState = nextState->process(streamWrapper, bufferManager);
  if (nextState == errorState)
    return -1;
  if (nextState == endState)
    return 1;
  return 0;
}

IntStreamParser::IntStreamParser()
{
  reader.valueState = &value;
  reader.endState = &end;
  reader.fractionState = NULL;
  reader.signedState = &sign;
  reader.errorState = &error;
  
  value.fractionState = NULL;
  value.endState = &end;

  sign.fractionState = &fraction;
  sign.errorState = &error;
  sign.valueState = &value;

  fraction.endState = &end;
  
  nextState = &reader;
}

void IntStreamParser::reset()
{
  nextState = &reader;
}


FloatStreamParser::FloatStreamParser()
{
  reader.fractionState = &fraction;
  value.fractionState = &fraction;
}