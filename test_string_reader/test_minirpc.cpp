#include <MiniRPC.h>
#include <StreamWrapper.h>

#include <StateMachine.h>
#include "BufferManager.h"

class TestMethod : public MiniRPCMethod
{
public:
  TestMethod(){
    setName("m");
  }
  virtual void prepare(){
    get(arg1, 10);
    get(arg2);
  }
  virtual void execute(){
    Serial.print(getName());
    Serial.print(":");
    Serial.println(arg1);
    Serial.println(arg2);
  }
private:
  char arg1[10];
  int arg2;
};

StreamWrapper in = StreamWrapper();
MiniRPCDispatcher d1 = MiniRPCDispatcher(&in);
/*
CharBufferManager charBufferManager = CharBufferManager();
StringStreamParser stringStreamParser = StringStreamParser();
char _test_value[30];
*/
TestMethod tm;

int main(void)
{
  in.setStream(&Serial);
  d1.registerMethod(&tm);
  Serial.setTimeout(1000);

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

  while(1){
    d1.update();
   // delay(500);
  }
    getch();
    getch();
  endwin();
  return 0;
}