#include <DummySerial.h>
#include <MiniRPC.h>
#include <StreamWrapper.h>

#include <StateMachine.h>
#include "BufferManager.h"


CharBufferManager out = CharBufferManager();
StringStreamParser sp = StringStreamParser();

IntStreamParser ip = IntStreamParser();
IntBufferManager number = IntBufferManager();

FloatBufferManager decimal = FloatBufferManager();
FloatStreamParser fp = FloatStreamParser();

int main(void)
{
  sp = StringStreamParser();
  sp = StringStreamParser();
  char output[40];
  StreamWrapper in = StreamWrapper();
  in.setStream(&Serial);
  int state = 0;
  
  out.init(output, 40);
  Serial.println("input str:");

  
  sp.setStreamWrapper(&in);
  sp.setBufferManager(&out);

  while((state = sp.process()) == 0){
    Serial.print("_"); 
    Serial.print(sp.test());
    Serial.print("_");
  }
  if (state == -1){
    Serial.println("Error");
  }else{
    Serial.println("Done");
    out.terminateStr();
    Serial.println(out.getBuffer());
  }
  
  number.init();
  Serial.println("input number:");
  ip.setStreamWrapper(&in);
  ip.setBufferManager(&number);
  while((state = ip.process()) == 0){
    Serial.print("="); 
  }
  if (state == -1){
    Serial.println("Error");
  }else{
    Serial.println("Done");
    Serial.println(number.getBuffer());
  }
  in.read();
  decimal.init();
  Serial.println("input decimal:");
  fp.setStreamWrapper(&in);
  fp.setBufferManager(&decimal);
  while((state = fp.process()) == 0){
    Serial.print("+"); 
  }
  if (state == -1){
    Serial.println("Error");
  }else{
    Serial.println("Done");
     Serial.println(decimal.getBuffer());
  }
  
  getch();
  getch();
  endwin();
  return 0;
}