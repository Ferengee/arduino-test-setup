#include <Arduino.h>

#include <ButtonMessageControl.h>
#include <BaseSenderReceiver.h>
#include "TestSenderReceiver.h"
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
}

void handleButtonCommand(CommandContext * context, digital_command_t cmd)
{
  Serial.print("handle button: ");
  Serial.println(cmd.value);
  
  od_command_header_t response; 
  response.label= LED;
  response.pwm.shape = SINE;
  response.pwm.offset = 125; 
  response.pwm.duration = 10;
  response.pwm.amplitude = 255, 
  response.pwm.period = 30;
  context->reply(&response);
}

void handleVibrateCommmand(CommandContext * context, pwm_command_t cmd)
{
  vibrate.set(cmd.shape, cmd.offset, cmd.duration, cmd.amplitude, cmd.period);
}
void handleLedCommand(CommandContext * context, pwm_command_t cmd)
{
  led.set(cmd.shape, cmd.offset, cmd.duration, cmd.amplitude, cmd.period);
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
      if ( incomingByte == 'l' ){
        int min, max;
        button.reportSendTime(min, max);
        Serial.print("min send time: ");
        Serial.println(min);
        
        Serial.print("max send time: ");
        Serial.println(max);
      } else if ( incomingByte != '\n') {
        runTest = true;
      }      
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
