#include "KnockStateMachine.ino"
#include <Schedulers.h>


CountdownTimer timeout;
Scheduler knockDoneTimer;

void confirmKnock(int token, void * data){
  Serial.println("  light: blink yellow once");
  timeout.reset(); 
}
void lightGreen(int token, void * data){
  Serial.println("  light: Green");
  timeout.reset(); 
  machine.receive(LISTEN, NULL);

}
void lightRed(int token, void * data){
  Serial.println("  light: Red");
  timeout.reset(); 
  machine.receive(LISTEN, NULL);

}
void lightPlayRecorded(int token, void * data){
  Serial.println("  light: Auto");
  timeout.reset(); 
  machine.receive(LISTEN, NULL);
}

void emitListen(void * data){
  Serial.println("emitListen...");
  machine.receive(LISTEN, NULL);
}

void emitGo(void * data){
  Serial.println("emitGo...");
  machine.receive(GO, NULL);
}


void setup(){
 
  timeout.start(30, emitGo, NULL);
  setup_machines();
 
 //exitScheduler.once(3000, scheduledExit, NULL);
 //shoutScheduler.every(100, shout, message);
 //countdown.start(4, liftoff, NULL);
 
 //schedulers.attach(exitScheduler);
 //schedulers.attach(shoutScheduler);
 //schedulers.attach(countdown);
}

void simulateTriggering(){
    for(int i=0; i < 10; i++){
      delay(10);
      timeout.trigger();
    }
}

void loop(){
 
  Serial.println("Knock once...");

  machine.receive(KNOCK, NULL);
  simulateTriggering();
 
  simulateTriggering();
  simulateTriggering();
    simulateTriggering();
  simulateTriggering();

      Serial.println("");

  Serial.println("Knock twice...");

  machine.receive(KNOCK, NULL);
  simulateTriggering();
  
 
  machine.receive(KNOCK, NULL);
  simulateTriggering();
  simulateTriggering();
  simulateTriggering();
  simulateTriggering();
  simulateTriggering();

  Serial.println("");

  Serial.println("Knock three times...");

    machine.receive(KNOCK, NULL);
  simulateTriggering();

 
 machine.receive(KNOCK, NULL);
  simulateTriggering();
   machine.receive(KNOCK, NULL);
  simulateTriggering();
  simulateTriggering();

  simulateTriggering();
  simulateTriggering();
  simulateTriggering();
        
  Serial.println("");

   //countdown.trigger();
   //schedulers.trigger();
  delay(1000);
 }
