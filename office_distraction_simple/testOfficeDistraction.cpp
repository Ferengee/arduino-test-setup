#include <Arduino.h>

#include <RFMessageControl.h>
#include <BaseSenderReceiver.h>
//#include <RFSenderReceiver.h>

#include "../Arduino/DummySerial.h"

uint8_t testbuffer[40];
char message[ ] = "Hallo";
uint8_t other = 2;
int button_pin = 3;
int led_pin = 13;
int incomingByte = 0;
bool runTest = false;

TestSenderReceiver trA = TestSenderReceiver();
TestSenderReceiver trB = TestSenderReceiver();

// BaseSenderReceiver tr = RFSenderReceiver();

RFMessageControl controlA = RFMessageControl(&trA);
RFMessageControl controlB = RFMessageControl(&trB);

void receiveMessageItem(MessageQueueItem item){
  Serial.print("Message received:") ;
  uint8_t message[MESSAGE_SIZE];
  uint8_t length = MESSAGE_SIZE;
  memset(message, 0, MESSAGE_SIZE);

  item.getMessage(message, &length);
  Serial.print(":");
  Serial.print(item.getMessageType());
  Serial.print(":");

  Serial.println((char *)message);
}

void receiveMessageItemA(MessageQueueItem item){
  Serial.println("    - A -");
  receiveMessageItem(item);
}
void receiveMessageItemB(MessageQueueItem item){
  Serial.println("    - B -");
  receiveMessageItem(item);
}

void notifyDiscartedItem(MessageQueueItem* item)
{
  Serial.print("Discarted item: ");
  Serial.print(item->getMessageId());
  Serial.print(":");
  Serial.println(item->getMessageType());
  

  
}

void setup(){
  Serial.begin(9600);
  Serial.setTimeout(10);
  pinMode(button_pin, INPUT);  
  digitalWrite(button_pin, HIGH);  //enable pullup resistor
  
  trA.m_other = &trB;
  trB.m_other = &trA;
  controlA.setChannelID(1);
  controlA.setMessageReceivedEventHandler(receiveMessageItemA);
  controlA.notifyDiscartedItem = notifyDiscartedItem;
  
  controlB.setChannelID(other);
  controlB.setMessageReceivedEventHandler(receiveMessageItemB);
  controlB.notifyDiscartedItem = notifyDiscartedItem;
  Serial.println("setup finished...");
}



void loop(){
   runTest = false;
   while((incomingByte = Serial.read()) > 0) {
      // read the incoming byte:;
      runTest = true;
      message[0] = incomingByte;
   }
  if(runTest){
    Serial.println("Running test...");
    if(!controlA.sendMessage(other, (uint8_t *)message, 6))
      Serial.println("Failed to allocate message...");
  }
  //Serial.println("update");  
//  delay(5);
  
  controlA.update();
  controlB.update();

//
}

int main()
{ 
  setup();
  for(;;){
    loop();
  }
  endwin();

}
