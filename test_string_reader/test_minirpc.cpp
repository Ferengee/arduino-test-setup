#include <DummySerial.h>
#include <MiniRPC.h>
#include <StreamWrapper.h>

#include <StateMachine.h>
#include "BufferManager.h"


StreamWrapper in = StreamWrapper();
MiniRPCDispatcher d1 = MiniRPCDispatcher(&in);
/*
CharBufferManager charBufferManager = CharBufferManager();
StringStreamParser stringStreamParser = StringStreamParser();
char _test_value[30];
*/
int main(void)
{
  in.setStream(&Serial);
/*
  stringStreamParser.setBufferManager(&charBufferManager);
  stringStreamParser.setStreamWrapper(&in);

  int state;
  charBufferManager.init(_test_value,30);
  stringStreamParser.reset();
  while((state = stringStreamParser.process()) == 0){
    Serial.print("_"); 
  }
  if (state == -1){
    Serial.println("Error");
  }else{
    Serial.println("Done");
    charBufferManager.terminateStr();
    Serial.println(charBufferManager.getBuffer());
  }
  */

  while(d1.update()){
    ;
  }
    getch();
    getch();
  endwin();
  return 0;
}