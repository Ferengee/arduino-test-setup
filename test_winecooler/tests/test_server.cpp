#include <Ethernet.h>

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

int testCreateWithEthernetServer(){
  EthernetServer http = EthernetServer(80);
  ApiServer server(http);
  
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

class MockEthernetServer : public EthernetServer
{
public:
  MockEthernetServer() : EthernetServer(80){}
  virtual EthernetClient available(){
    return client;
  }
  
private:
  EthernetClient client;
};

int testHandleIncommingRequests(){
  MockEthernetServer http;
  ApiServer server(http);
  server.handleIncommingRequests();
  
  // should delegate the request to handlers if the request is valid
  return 0;

  }

int testCreateApiRequest(){
  ApiRequest request;
  char key[1] = "";
  char * actual = request.getKey();

  check(request.method() == NONE, "Expected uninitialized request to have no type");
  check(request.intData() == -1, "Expected uninitialized request to -1 intData()");
  check(strstr(key, actual) == key, "Expected uninitialized request to return empty string for getKey(), got: %s", actual);

  return 0;
error:
  return 1;
  
}

int testApiRequestRoot(){
  MockStream stream;
  char getstr[] = "GET / HTTP/1.1\n";
  stream.setSourceString(getstr, 15);

  ApiRequest request;
  request.setStream(&stream);


  char * actual = request.getKey();
  check(strcmp("", actual) == 0, "Expected getKey() to return:'', got '%s'",  actual);
  check(request.valid(), "Expected the request parsed from: '%s' to be valid.\n", getstr);
  check(request.valid(), "Expected valid to return the same the second time.\n");
  check(request.getInstanceId() == -1, "Expected no id to be found.\n");
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
  request.valid();
  
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
  bool is_valid = false;
  
  stream.setSourceString(idstr, 26);
  
  ApiRequest request;

  check(request.getInstanceId() == -1, "Expected uninitialized request to return an error for getInstanceId()");
 
  request.setStream(&stream);

  is_valid = request.valid();
  
  check(is_valid, "Expected the request parsed from: '%s' to be valid.\n", idstr);
  id = request.getInstanceId();
  check(id == 17, "Expected id to be set to 17, got %d", id);
  
  stream.setSourceString(noidstr, 25);
  request.valid();


  return 0;
error:
  return 1;  
}

int testApiRequestIntData(){
  ApiRequest request;

  char requestData[] = "POST /some/key/0 HTTP/1.2\r\n"
    "Host: localhost:8080\r\n"
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0\r\n"
    "Accept: application/json, text/plain, */*\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 3\r\n"
    "Origin: null\r\n"
    "Connection: keep-alive\r\n"
  "\r\n"
  "235\r\n";
    
  MockStream stream;
  int actual = -2;
  int expected = -1;
  
  stream.setSourceString(requestData, sizeof(requestData));
  actual = request.intData();
  check(actual == expected, "Expected uninitialized request to return %d for intData(), got: %d", expected, actual);
  
  request.setStream(&stream);

  request.valid();
  
  expected = 235;
  actual = request.intData();
  check(actual == 235, "Expected intData() to return: %d, got: %d ", expected, actual);

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

  request.valid();
  
  char * actual =  request.getKey();
  
  check(strcmp(key1, actual) == 0, "Expected getKey() to return: %s, got %s", key1, actual);
  
  actual =  request.getKey();
  check(strcmp(key1, actual) == 0, "Expected getKey() to return the same key if requested more than once (expected: %s, got %s)", key1, actual);

  stream.setSourceString(key2str, 23);
  request.initialize();
  check(strcmp(key2, request.getKey()) == 0, "Expected getKey() to return the key, got: %s", request.getKey());

  return 0;
error:
  return 1;
}


int main(){
  int e = 0;
  printf("running server tests:");
  
  e = e || testCreate();
  e = e || testCreateWithEthernetServer();
  e = e || testOn();
  e = e || testHandle();
  e = e || testHandleIncommingRequests();

  e = e || testCreateApiRequest();
  e = e || testApiRequestRoot();
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