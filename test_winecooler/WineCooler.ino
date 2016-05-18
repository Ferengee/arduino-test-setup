#include <Schedulers.h>
#include "lib.c"

#define LINK_COUNT 3

Scheduler exitScheduler;
Scheduler sampler;

Schedulers schedulers;

AggregatingLog temp[LINK_COUNT];
AggregatingLog pressure[LINK_COUNT];

AnalogSampler tempSensor;
AnalogSampler pressureSensor;


char output[40];

void printChain(){
  
  Serial.println("temp:" );

  temp->chainToJSON(output);
  Serial.println(output);

  Serial.println("pressure:" );
  pressure->chainToJSON(output);

  Serial.println(output);
}

void scheduledExit(void * sched){
  printChain();
 
  Serial.println( "exit!\n");
  exit(0); 
}

void takeSample(void * nothing){
  /*analogRead(A0);
  delay(10);
  temp->add(analogRead(A0));
  delay(10);
  
  analogRead(A1);
  delay(10);
  pressure->add(analogRead(A1));
  delay(10);
*/
 tempSensor.sample();
 pressureSensor.sample();
  
 printChain();
}
  
void setup(){
 AggregatingLog::linkChain(temp, LINK_COUNT);
 AggregatingLog::linkChain(pressure, LINK_COUNT);
 
 tempSensor.pin = A0;
 pressureSensor.pin = A1;
 
 tempSensor.log = temp;
 pressureSensor.log = pressure;

 exitScheduler.once(3000, scheduledExit, NULL);
 sampler.every(100, takeSample, NULL);
 
 
 schedulers.attach(exitScheduler);
 schedulers.attach(sampler);
}

 void loop(){
   delay(50);
   schedulers.trigger();
 }
