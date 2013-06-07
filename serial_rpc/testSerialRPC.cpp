#include <Arduino.h>

#include "../Arduino/DummySerial.h"
#include <SerialRPC.h>


HelloMethod hm = HelloMethod();

class TestMethod : public SerialRPCMethod
{
public:    
  virtual void execute(){ 
    Serial.print(m_arg1);  
    Serial.print(m_arg2);
  }
  virtual bool init(){ 
    getNextValue(&m_arg1); 
    return getNextValue('O', (char *)&m_arg2, &m_arg2len);
  }
  TestMethod(){
    m_name = "Test"; 
    m_arg2len = 10;
  }
private:
  int m_arg1;
  char m_arg2[30];
  int m_arg2len;
};

TestMethod t1 = TestMethod();

int main(){
  Serial.println("test");
  if(hm.init()){
    Serial.println("init...");
    Serial.println(hm.getName());
    hm.execute();
  }
  Serial.println("test class");
  if(!t1.init())
    t1.printError();
  t1.execute();
 Serial.println("done...");
  for(;;){}
}
