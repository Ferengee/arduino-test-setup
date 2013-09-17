#include <Arduino.h>

#include <ButtonMessageControl.h>
#include <BaseSenderReceiver.h>
#include <PWMControl.h>

#include "../Arduino/DummySerial.h"

uint8_t testbuffer[40];
char message[ ] = "Hallo";
int button_pin = 3;
int led_pin = 13;
int incomingByte = 0;
bool runTest = false;

TestSenderReceiver trA = TestSenderReceiver();
TestSenderReceiver trB = TestSenderReceiver();

// BaseSenderReceiver tr = RFSenderReceiver();
PWMControl led = PWMControl(5);
PWMControl vibrate = PWMControl(6);

ButtonMessageControl button = ButtonMessageControl(&trA);
ButtonMessageControl master = ButtonMessageControl(&trB);



void notifyDiscartedItem(MessageQueueItem* item)
{
  Serial.print("Discarted item: ");
  Serial.print(item->getMessageId());
  Serial.print(":");
  Serial.println(item->getMessageType());
}

void handleButtonCommand(bool value)
{
  Serial.print("handle button: ");
  Serial.println(value);
  master.sendLedCommand(1, SINE, 0, 200, 255, 30);
}

void handleVibrateCommmand(uint8_t shape, uint8_t offset, uint8_t duration, uint8_t amplitude, uint8_t period)
{
  led.set(shape, offset, duration, amplitude, period);
}
void handleLedCommand(uint8_t shape, uint8_t offset, uint8_t duration, uint8_t amplitude, uint8_t period)
{
  vibrate.set(shape, offset, duration, amplitude, period);
}


void setup(){
  Serial.begin(9600);
  Serial.setTimeout(10);
  pinMode(button_pin, INPUT);  
  digitalWrite(button_pin, HIGH);  //enable pullup resistor
  
  trA.m_other = &trB;
  trB.m_other = &trA;
  button.setChannelID(1);
  button.handleVibrateCommand = handleVibrateCommmand;
  button.handleLedCommand = handleLedCommand;
  
  button.notifyDiscartedItem = notifyDiscartedItem;
  
  master.setChannelID(MASTER);
  master.handleButtonCommand = handleButtonCommand;
  master.notifyDiscartedItem = notifyDiscartedItem;
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
    if(!button.sendButtonEvent(true))
      Serial.println("Failed to allocate message...");
  }
  //Serial.println("update");  
//  delay(5);
  
  button.update();
  master.update();
  led.update();
  vibrate.update();
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
