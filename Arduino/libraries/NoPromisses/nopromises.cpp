#include "nopromisses.h"
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
