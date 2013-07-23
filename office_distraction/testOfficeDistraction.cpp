#include <Arduino.h>

#include <RFMessageControl.h>
#include <BaseSenderReceiver.h>
#include "../Arduino/DummySerial.h"

char message[ ] = "Hallo";
uint8_t other = 1;
int button_pin = 3;
int led_pin = 13;
int incomingByte = 0;
bool runTest = false;
TestSenderReceiver tr = TestSenderReceiver();
RFMessageControl control = RFMessageControl(&tr);

void receiveMessageItem(MessageQueueItem * item){
  Serial.print("Message received:") ;
  uint8_t message[MESSAGE_SIZE];
  uint8_t length = MESSAGE_SIZE;
  item->getMessage(message, &length);
  Serial.println((char *)message);
}

void setup(){
  Serial.begin(9600);
  pinMode(button_pin, INPUT);  
  digitalWrite(button_pin, HIGH);  //enable pullup resistor
  control.setChannelID(1);
  control.setMessageReceivedEventHandler(receiveMessageItem);
  Serial.println("setup finished...");
}



void loop(){
   runTest = false;
   while((incomingByte = getch()) > 0) {
      // read the incoming byte:;
      runTest = true;
   }
  if(runTest){
    Serial.println("Running test...");
    control.sendMessage(other, (uint8_t *)message, 6);
  }
  //Serial.println("update");  
  delay(5);
  control.update();
  delay(5);
}

int main()
{ 
  setup();
  for(;;){
    loop();
  }
  endwin();

}
