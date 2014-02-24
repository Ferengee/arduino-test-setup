#include <Arduino.h>

#include <ButtonMessageControl.h>
#include <BaseSenderReceiver.h>
#include "TestSenderReceiver.h"
#include <PWMControl.h>
#include "stdio.h"
#include "../Arduino/DummySerial.h"

typedef struct message_s {
  message_s() : id(0), channel(1), retries(2), length(3){}
  uint8_t id;
  uint8_t channel;
  uint8_t retries;
  uint8_t length;
} message_t;

uint8_t testbuffer[40];
char message[ ] = "Hallo";
int button_pin = 3;
int led_pin = 13;
int incomingByte = 0;
bool runTest = false;

TestSenderReceiver trA = TestSenderReceiver();

// BaseSenderReceiver tr = RFSenderReceiver();


void setup(){
  trA.init(0,0,2000);
  Serial.begin(9600);
  Serial.setTimeout(10);
 

 
  Serial.println("setup finished...");
}



void loop(){
   runTest = false;
   while((incomingByte = Serial.read()) > 0) {
      // read the incoming byte:;
    
      if ( incomingByte != '\n') {
        runTest = true;
      }      
   }
  if(runTest){
    Serial.println("Running test...");
    uint8_t buf[80];
    uint8_t len = 80;
    message_t out;
    
    if (trA.have_message()){
      Serial.println("get message:");
      trA.get_message((uint8_t *)buf, &len);
            message_t * msg = (message_t *)buf;
      printf("id:%d, channel:%d, retries:%d, length:%d", msg->id, (int)msg->channel, (int)msg->retries, (int)msg->length);
      Serial.println("---");
    } else {
      Serial.println("send message...");
      out.id = 32;
      out.channel = 2;
      out.retries = 4;
      trA.send((uint8_t *)&out, sizeof(message_t));
    }
  }
  //Serial.println("update");  
//  delay(5);
  
}

int main()
{ 
  setup();
  for(;;){
    loop();
  }
  endwin();

}
