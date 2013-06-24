#include "StateMachine.h"
#include <DummySerial.h>



int main(void)
{
  char output[40];
  
  StreamWrapper in = StreamWrapper();
  in.setStream(&Serial);
  
  CharBufferManager bm = CharBufferManager();
  bm.init(output, 40);
  
  ReaderStartState reader = ReaderStartState();
  ReaderState error = ReaderEndState();
  ReaderState endstate = ReaderEndState();

  ReaderNormalState normal = ReaderNormalState();
  ReaderEscapedState escaped = ReaderEscapedState();
  
  ReaderState * nextState;
  
  reader.errorState = &error;
  reader.normalState = &normal;
  reader.startToken = 0;
  
  normal.errorState = &error;
  normal.escapedState = &escaped;
  normal.endState = &endstate;
  normal.endToken = '(';
  
  escaped.normalState = &normal;
  escaped.errorState = &error;

  
  nextState = &reader;
  while (nextState != &error){
    nextState = nextState->process(&in, &bm);
    if(nextState == &escaped)
      Serial.print("'");
    else if (nextState == &normal)
      Serial.print("_");
    else
      Serial.print(".");
    if(nextState == &endstate)
      break;
  }
  if(nextState == &error){
    Serial.println(" Error");
  }else{
    Serial.println(" Done");
  }
  bm.terminateStr();
  Serial.println(bm.getBuffer());
  return 0;
}