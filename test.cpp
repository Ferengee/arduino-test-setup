#include <iostream> 
#include <sys/time.h>
#include <string.h>

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
 
  void resolve(void * data);
  void reject(char * reason);
private:
  OnFulfilled onFulfilled;
  OnRejected onRejected;
  void * data;
  char * reason;
  DeferredClosure * closure;
  //Todo: use boolean flags because we can be resolved with a NULL
  bool isResolved(){
    return (this->data != NULL || this->reason != NULL); 
  }
};

/*
 * Extend the DeferredClosure class to implement a specific context which
 * the OnFulfilled can use to get references to the outside world
 * 
 * The closure carries a prommise which is returned by Deferred::then() to chain then calls
 */
class DeferredClosure
{
public:
  DeferredClosure();
  void throwException(char * exception);
  bool catchException(char** message);

  Deferred prommise;
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
  
  return &closure->prommise; 
}

Deferred * Deferred::then(DeferredClosure* closure, OnFulfilled onFulfilled)
{
  return this->then(closure, onFulfilled, NULL);
}

void Deferred::resolve(void * data){
  void * output = NULL;
  char * reason = NULL;
  if (this->isResolved())
    return;
  this->data = data;
  if(this->onFulfilled != NULL){
    output = this->onFulfilled(this->closure, data); 
  }
  //resolve a prommise which is not (yet) chained
  if(this->closure == NULL)
    return;
  
  if(!this->closure->catchException(&reason)){
    // if the onFulfilled returns the closures prommise, it takes
    // responsibility for resolving it
    if(&this->closure->prommise != output)
      this->closure->prommise.resolve(output);
  }else{
    this->closure->prommise.reject(reason);
  }
  
  
}

void Deferred::reject(char * reason){
  void * output = NULL;

  if (this->isResolved())
    return;
  this->reason = reason;
  if(this->onRejected != NULL){
    output = onRejected(reason); 
  }
  if(this->closure != NULL){
    if(&this->closure->prommise != output)
      this->closure->prommise.reject(reason);    
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

unsigned long millis(void){
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
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
  return &closure->prommise;
}

void * printFriend(DeferredClosure* closure, void * data){
  static char result[] = "print friend\n";
  
  cout << data;
  //Deferred * myFriend = (Deferred *)data;
  //myFriend->then(NULL, printHello);
  return result;
}

int main(int argc, const char* argv[]){
  Deferred getUser;

  LookUpFriendClosure helloClosure;
  LookUpFriendClosure lookUpFriendClosure;
  DeferredClosure printFriendClosure;
  DeferredClosure bye;

  DeferredClosure handleError;
  
  getUser.then(&lookUpFriendClosure, lookUpFriend)->then(&printFriendClosure, printFriend)->then(&helloClosure, printHello)->then(&bye, printBye)->then(&handleError, NULL, handleErrorFn);
  //getUser()->then(getBestFriend)->then(&username, printUserName)->then(&handleError, NULL, handleErrorFn);
  
  
  char user[255] = "bas";
  char my_friend[] = "ferengee";

  user[254] = 0;
  
//  printFriendClosure.prommise.resolve(my_friend);

  
  if (argc > 1)
    memcpy(user, argv[1], 254);
  unsigned long now = millis();
  while(true){
      if(millis() - now > 1000){
        getUser.resolve(user);
        break; 
      }
  }
  
  while(true){
      if(millis() - now > 2000){
        if (strcmp("bas", user) == 0){
          printFriendClosure.prommise.resolve(my_friend);
        }
        break; 
      }
  }
  
  
}