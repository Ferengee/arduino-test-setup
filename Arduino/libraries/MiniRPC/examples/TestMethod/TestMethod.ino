#include <MiniRPC.h>
#include <StreamWrapper.h>
#include <StateMachine.h>
#include <BufferManager.h>
#include <Arduino.h>

class ListMethods : public MiniRPCMethod
{
public:
  ListMethods(){
    setName("listMethods");
  }
  virtual void prepare(){}
  virtual void execute(){
    MiniRPCMethod * list[MAX_METHOD_COUNT];
    int length = 0;
    dispatcher->getMethodList(list, length);
    int i;
    Serial.print("[");
    for (i=0; i < length; i++){
      Serial.print(list[i]->getName());
      if(i < length -1)
        Serial.print(", ");
    }
    Serial.println("]");
  }
};
class RemoteControlledLed : public MiniRPCMethod
{
 
public:
  RemoteControlledLed(){
    setName("led");
    led = 11;
  }
 
  virtual void prepare(){
    get(led);
    get(intensity);
  }

  virtual void execute(){
    pinMode(led, OUTPUT);
    analogWrite(led, intensity);
  }
  
  virtual void error(){
    Serial.println("Error in parsing arguments");
    Serial.println("Expected: led(<pin>, <intensity>)");
  }
private:
  int intensity;
  int led;
};

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
  float arg2;
};

StreamWrapper in = StreamWrapper();
MiniRPCDispatcher d1 = MiniRPCDispatcher(&in);
/*
CharBufferManager charBufferManager = CharBufferManager();
StringStreamParser stringStreamParser = StringStreamParser();
char _test_value[30];
*/
TestMethod tm;
ListMethods lm;
RemoteControlledLed rcl;


void setup()
{
  Serial.begin(9600);
  in.setStream(&Serial);
  d1.registerMethod(&tm);
  d1.registerMethod(&lm);
  d1.registerMethod(&rcl);

  
  Serial.println("{'class': 'Event', 'message': 'setup finished'}");
}

void loop(){
    d1.update();
   // delay(500);
}

