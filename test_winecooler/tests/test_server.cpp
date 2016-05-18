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

int testCreateApiRequest(){
  ApiRequest request;
  int data;
  char key[1] = "";
  char * actual = request.getKey();

  check(request.method() == NONE, "Expected uninitialized request to have no type")
  check(request.intData(&data) == -1, "Expected uninitialized request to return an error for intData()")
  check(strstr(key, actual) == key, "Expected uninitialized request to return empty string for getKey(), got: %s", actual);

  return 0;
error:
  return 1;
  
}

int testApiRequestMethod(){
  MockStream stream;
  char getstr[] = "GET /variable/0 HTTP/1.1\n";
  char putstr[] = "PUT /variable/0 HTTP/1.1\n";

  stream.setSourceString(getstr, 26);

  
  ApiRequest request;
  check(request.method() == NONE, "Expected uninitialized request to have no type");
  
  request.setStream(&stream);
  request.initialize();
  
  check(request.method() == GET, "Expected request to read method from stream");
  
  stream.setSourceString(putstr, 26);
  request.initialize();

  check(request.method() == PUT, "Expected request to read method from stream");
  check(request.method() == PUT, "Expected request cache the read value");

  
  return 0;
error:
  return 1;
  
}

int testApiRequestGetInstanceId(){
  MockStream stream;
  char idstr[] = "GET /variable/17 HTTP/1.1\n";
  char noidstr[] = "PUT /variable/ HTTP/1.1\n";
  int id = -2;
  stream.setSourceString(idstr, 26);
  
  ApiRequest request;

  check(request.getInstanceId() == -1, "Expected uninitialized request to return an error for getInstanceId()")
 
  request.setStream(&stream);

  request.initialize();
  
  id = request.getInstanceId();
  check(id == 17, "Expected id to be set to 17, got %d", id);
  
  stream.setSourceString(noidstr, 25);
  request.initialize();


  return 0;
error:
  return 1;  
}

int testApiRequestIntData(){
  ApiRequest request;
  int data;

  check(request.intData(&data) == -1, "Expected uninitialized request to return an error for intData()")
  return 0;
error:
  return 1;
}

int testApiRequestGetKey(){
  char key1[] = "variable";
  char key2[] = "sensor";

  MockStream stream;
  char key1str[] = "GET /variable/17 HTTP/1.1\n";
  char key2str[] = "GET /sensor/17 HTTP/1.1\n";

  stream.setSourceString(key1str, 26);
  
  ApiRequest request;
    
  request.setStream(&stream);

  request.initialize();
  check(strstr(key1, request.getKey()) == key1, "Expected getKey() to return the key");

  stream.setSourceString(key2str, 23);
  request.initialize();
  check(strstr(key2, request.getKey()) == key2, "Expected getKey() to return the key");

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
  e = e || testCreateApiRequest();

  e = e || testApiRequestGetInstanceId();
  e = e || testApiRequestIntData();
  e = e || testApiRequestGetKey();
  e = e || testApiRequestMethod();
  
  if(e > 0)
    printf("   FAIL!\n");
  else
    printf("   OK!\n");

  return 0;
}