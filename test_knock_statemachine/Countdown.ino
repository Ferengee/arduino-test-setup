#include <Schedulers.h>

void scheduledExit(void * sched){
  Serial.println( "exit!\n");
  exit(0); 
}

void shout(void * message){
  Serial.println( (char *)message);
}

/*
 * Wrap / encapsulate
 * - interrup handlers
 * - timed events / wait
 * 
 * 
 */


void liftoff(void * scheduler){
  Serial.println("liftoff...");
}


Scheduler exitScheduler;
Scheduler shoutScheduler;
Schedulers schedulers;
CountdownTimer countdown;

char message[] = "hello world!\n";

  
void setup(){
   

  /* unsigned long now = millis();
 while(true){
      if(millis() - now > 1000){
        getUser.resolve(user);
        break; 
      }
  }
  
  while(true){
      if(millis() - now > 2000){
        printFriendClosure.promise.resolve(my_friend);
        break; 
      }
  }
  */
 
 exitScheduler.once(3000, scheduledExit, NULL);
 shoutScheduler.every(100, shout, message);
 countdown.start(4, liftoff, NULL);
 
 schedulers.attach(exitScheduler);
 schedulers.attach(shoutScheduler);
 schedulers.attach(countdown);
}

 void loop(){
   delay(50);
   //countdown.trigger();
   schedulers.trigger();
 }
