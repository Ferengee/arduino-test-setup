#include "server/server.h"
#include "handler/handler.h"

#include "dbg.h"
#include <DummySerial.h>
#include <MockStream.h>


int testCreate(){
  ApiServer server;
  
  check(&server != NULL, "bogus check")

  return 0;
error:
  return 1;
  
}

int testOn(){
  const char* location = "some/path";
  ApiServer server;
  AbstractHandler handler;
  
  server.on(location)->use(&handler);

  check(handler.getLocation() == location, "Expected location to be set on the handler");

  return 0;
error:
  return 1;
  
}

class MockHandler : public AbstractHandler {

public:
  MockHandler(){
    handleCalled = false;
  }
  virtual bool canHandle(const char * location) {
    this->location = location;
    return true;
  }
  
  virtual bool handle(ApiRequest * request){
    handleCalled = true;
    return true;
  }
  
  bool handleCalled;
  bool canHandleCalled(){
    return this->location != NULL;
  };
};

int testHandle(){
  const char* location = "some/path";
  const char* path1 = "some/path/rest";
  const char* path2 = "not/matching/path";
  
  ApiServer server;
  MockHandler handler;
  
  check(handler.canHandleCalled() == false, "Error in test");
  server.on(location)->use(&handler);
  server.handle(path1);
  
  check(handler.canHandleCalled() == true, "Expect handler.canHandle() to have been called");
   
  server.handle(path2);

  return 0;
error:
  return 1;
  
}

int main(){
  int e = 0;
  printf("running server tests:");
  
  e = e || testCreate();
  e = e || testOn();
  e = e || testHandle();

  if(e > 0)
    printf("   FAIL!\n");
  else
    printf("   OK!\n");

  return 0;
}