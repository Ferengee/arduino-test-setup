#include <nopromisses.h>

class LookUpFriendClosure : public DeferredClosure {
public:
  Deferred myFriend;
};

void * printHello(DeferredClosure* closure, void * data){
  Serial.println( "hello ");
  if(data != NULL){
    Serial.println( (char *)data);
    //*(char *)data = 'D'; 
  }
  return data;
}

void * printBye(DeferredClosure* closure, void * data){
  Serial.println( " goodbye ");
  if(data != NULL)
    Serial.println( (char *)data);
  return data;
}

void * handleErrorFn(const char * reason){
  if(reason != NULL);
    Serial.print("Error: ");
    Serial.println( reason);
  return NULL; 
}

void * lookUpFriend(DeferredClosure* closure, void * data){
  static char error[] = "User has no friends.\n";
  
  bool succes = (strcmp("bas", (char *)data) == 0);
  if(!succes){
    closure->throwException(error);
  }
  return &closure->promise;
}

void * printFriend(DeferredClosure* closure, void * data){
  static char result[] = "print friend\n";
  
  Serial.println((char *)data);
  //Deferred * myFriend = (Deferred *)data;
  //myFriend->then(NULL, printHello);
  return result;
}

void scheduledExit(void * sched){
  Serial.println("exit!");
  exit(0); 
}

void shout(void * message){
  Serial.println( (char *)message);
}


Deferred getUser;

DeferredClosure closures[4];

DeferredClosure handleError;

char user[255] = "bas";
char my_friend[] = "ferengee\n";



char message[] = "hello world!\n";

  
void setup(){
  user[254] = 0;

  Deferred * result = &getUser;

  OnFulfilled actions[] = {lookUpFriend, printFriend, printHello, printBye};
  result = result->andThenAndThen(closures, actions, 4);
  result->fail(&handleError, handleErrorFn);


  getUser.resolve(user);
  closures[1].promise.resolve(my_friend);

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
 
}

 void loop(){
   delay(50);
 }
  
  /*
   * a then(closure, waitXSeconds)
   * 
   * waitXSecons(closure, input){
   *   ScheduledClosure * scheduledClosure = (ScheduledClosure *)closure;
   *   scheduledClosure->sheduler.once(input, resolvePromise);
   *   return closure->promise; 
   * }
   * 
   * resolvePromise(scheduler){
   *    SchedulerWithPromise * schedulerWithPromise = (SchedulerWithPromise)scheduler;
   *    schedulerWithPromise->resolve();
   * }
   * 
   * waitForScheduler(closure, input){
   *   closure->scheduler.result = input;
   *   closure->scheduler.resolveAt(3000);
   * 
   *   return closure->promise; 
   * }
   */
  
  /*
   * Build a self forfilling (or rejecting) promise
   * of which the timeout can be set by a OnFulfilled or OnRejected handler of the previous then() call
   */
