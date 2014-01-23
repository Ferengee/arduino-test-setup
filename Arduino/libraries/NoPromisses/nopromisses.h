#ifndef NoPromisses_h
#define NoPromisses_h

#include <Arduino.h>
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

#endif