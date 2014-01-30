#include <Arduino.h>
#include <EventChannel.h>

class TestListener : public Listener
{
public:
  virtual void receive(int event, void * data);
  int id;
};

void TestListener::receive(int event, void* data)
{
    Listener::receive(event, data);

    Serial.print(id);
    
    Serial.print(" : ");
    
    Serial.println(event);
}

EventChannel channel;
TestListener test1;
TestListener test2;

int main(){
  test1.id = 1;
  test2.id = 2;
  
  channel.addListener(test1);
  channel.addListener(test2);
  
  channel.send(7, NULL);
  return 0; 
}