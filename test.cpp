#include <iostream>       
#include <stdlib.h>

#include <string.h>
#include "scheduler.h"

using namespace std;

class Deferred;
class DeferredClosure;
/*
 * A deferred function can be used as onFulfilled handler
 * should return true on succesfull evaluation
 * or false on error
 * 
 * the function is called with the result argument of resolve() 
 * as input
 * and can supply its output via the output pointer
 * 
 * supply a reason on failure
 * 
 */
typedef void * (* OnFulfilled) (DeferredClosure* closure, void * input);
/*
 *  or as onRejected handler
 *  is called with the reason which a onFulfilled handler gives on failure
 */
typedef void * (* OnRejected) (const char * reason);

class Deferred
{
public:
  Deferred();
  Deferred * then(DeferredClosure * closure, OnFulfilled onFulfilled, OnRejected onRejected);
  Deferred * then(DeferredClosure * closure, OnFulfilled onFulfilled);
  Deferred * andThenAndThen(DeferredClosure * closures, OnFulfilled * actions, int actionCount);
  Deferred * fail(DeferredClosure * closure, OnRejected onRejected){ return then(closure, NULL, onRejected); };
  
  void resolve(void * data);
  void reject(char * reason);
  
private:
  OnFulfilled onFulfilled;
  OnRejected onRejected;
  void * data;
  bool isResolved;
  char * reason;
  bool isRejected;
  DeferredClosure * closure;
  bool isSettled(){
    return (this->isRejected || this->isResolved); 
  }
};

/*
 * Extend the DeferredClosure class to implement a specific context which
 * the OnFulfilled can use to get references to the outside world
 * 
 * The closure carries a promise which is returned by Deferred::then() to chain then calls
 */
class DeferredClosure
{
public:
  DeferredClosure();
  void throwException(char * exception);
  bool catchException(char** message);

  Deferred promise;
private:
  char * thrownException;
  bool exceptionIsThrown;
};

class LookUpFriendClosure : public DeferredClosure {
public:
  Deferred myFriend;
};

DeferredClosure::DeferredClosure()
{
  exceptionIsThrown = false;
  thrownException = NULL;
}

bool DeferredClosure::catchException(char ** message)
{
  if(this->exceptionIsThrown)
    *message = this->thrownException;
  return this->exceptionIsThrown;
}
void DeferredClosure::throwException(char* exception)
{
  this->thrownException = exception;
  this->exceptionIsThrown = true;
}


Deferred::Deferred(){
  this->data = NULL;
  this->reason = NULL;
  this->closure = NULL;
  this->onFulfilled = NULL;
  this->onRejected = NULL;
  this->isRejected = false;
  this->isResolved = false;
}

Deferred * Deferred::then(DeferredClosure* closure, OnFulfilled onFulfilled, OnRejected onRejected)
{ 
  this->onFulfilled = onFulfilled;
  this->onRejected = onRejected;
  this->closure = closure;
  
  if(this->data != NULL)
    onFulfilled(this->closure, this->data);
  if(this->reason != NULL)
    onRejected(this->reason);
  
  return &closure->promise; 
}

Deferred * Deferred::then(DeferredClosure* closure, OnFulfilled onFulfilled)
{
  return this->then(closure, onFulfilled, NULL);
}

void Deferred::resolve(void * data){
  void * output = NULL;
  char * reason = NULL;
  if (this->isSettled())
    return;
  this->data = data;
  this->isResolved = true;
  if(this->onFulfilled != NULL){
    output = this->onFulfilled(this->closure, data); 
  }
  //resolve a promise which is not (yet) chained
  if(this->closure == NULL)
    return;
  
  if(!this->closure->catchException(&reason)){
    // if the onFulfilled returns the closures promise, it takes
    // responsibility for resolving it
    if(&this->closure->promise != output)
      this->closure->promise.resolve(output);
  }else{
    this->closure->promise.reject(reason);
  }
}

Deferred * Deferred::andThenAndThen(DeferredClosure * closures, OnFulfilled * actions, int actionCount)
{
  if (actionCount < 1)
    return this;
  
  int i;
  
  Deferred * result = this;
  
  for (i = 0; i < actionCount; i++){
    result = result->then(&closures[i], actions[i]);
  }
  return result;
}

void Deferred::reject(char * reason){
  void * output = NULL;
  
  if (this->isSettled())
    return;
  this->reason = reason;
  this->isRejected = true;
  if(this->onRejected != NULL){
    output = onRejected(reason); 
  }
  if(this->closure != NULL){
    if(&this->closure->promise != output)
      this->closure->promise.reject(reason);    
  }
}

void * printHello(DeferredClosure* closure, void * data){
  cout << "hello ";
  if(data != NULL){
    cout << (char *)data;
    cout << "\n";
    //*(char *)data = 'D'; 
  }
  return data;
}

void * printBye(DeferredClosure* closure, void * data){
  cout << " goodbye ";
  if(data != NULL)
    cout << (char *)data;
  return data;
}

void * handleErrorFn(const char * reason){
  if(reason != NULL);
    cout << "Error: ";
    cout << reason;
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
  
  cout << data;
  //Deferred * myFriend = (Deferred *)data;
  //myFriend->then(NULL, printHello);
  return result;
}

void scheduledExit(void * sched){
  cout << "exit!\n";
  exit(0); 
}

void shout(void * message){
  cout << (char *)message;
}

/*
 * Wrap / encapsulate
 * - interrup handlers
 * - timed events / wait
 * 
 * 
 */
Deferred * andThen(Deferred * start, DeferredClosure * closures, OnFulfilled * actions, int actionCount)
{
  if (actionCount < 1)
    return start;
  
  int i;
  
  Deferred * result = start;
  
  for (i = 0; i < actionCount; i++){
    result = result->then(&closures[i], actions[i]);
  }
  return result;
}

int main(int argc, const char* argv[]){
  Deferred getUser;

  DeferredClosure closures[4];
  
  DeferredClosure handleError;

  Deferred * result = &getUser;

  OnFulfilled actions[] = {lookUpFriend, printFriend, printHello, printBye};
  result = result->andThenAndThen(closures, actions, 4);
  result->fail(&handleError, handleErrorFn);
  
  
  char user[255] = "bas";
  char my_friend[] = "ferengee\n";

  user[254] = 0;
  

  
  if (argc > 1)
    memcpy(user, argv[1], 254);
  
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
 Scheduler exitScheduler;
 Scheduler shoutScheduler;
 Schedulers schedulers;
 
 char message[] = "hello world!\n";
 
 exitScheduler.once(3000, scheduledExit, NULL);
 shoutScheduler.every(100, shout, message);
 
 schedulers.attach(&exitScheduler);
 schedulers.attach(&shoutScheduler);

 while(true){
   schedulers.trigger();
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
}