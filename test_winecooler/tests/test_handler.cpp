#include "handler/handler.h"
#include "dbg.h"
#include <DummySerial.h>
#include <MockStream.h>


int testMockSerialRead(){
  
  MockStream stream;
  char source[] = "543";
  stream.setSourceString(source, 3);
  int i;
  int val=2;
  
  for (i=0; i<3; i++){
    check(stream.read() == source[i], "Expect stream.read to return %c", source[i]); 
  }
  
  val = stream.read();
  check(val == -1, "Expect to return -1 if the stream has ended, got %d", val);

  val = stream.read();
  val = stream.read();

  check(val == -1, "Expect to return -1 if the stream has ended, got %d", val);
  return 0;
error:
  return 1;
}

int testCreate(){
  AbstractHandler handler;
  check(handler.getLocation() == NULL, "Expected handler initialize location as NULL");
  
  return 0;
error:
  return 1;
}

const char * loc = "/key/0";


int testSetLocation(){
  AbstractHandler handler;

  handler.setLocation(loc);
  check(handler.getLocation() == loc, "Expected handler to remember location");

  return 0;
error:
  return 1;
}

int testLink(){
  HandlerList item1;
  HandlerList item2;
  check(item1.handler == NULL, "Expected handler to be initialized as NULL");
  check(item1.next() == NULL, "Expected next item to be initialized as NULL");
  check(item1.hasNext() == false, "Expected item1 not to have another link");
  
  
  item1.link(&item2);
  
  check(item1.hasNext() == true, "Expected item1 to have another link");

  
  check(item1.next() == &item2, "Expected handlerB to be the next one");

  return 0;
error:
  return 1;
  
}


int main(){
  int e = 0;
  printf("running handler tests:");
  
  e = e || testMockSerialRead();
  e = e || testCreate();
  e = e || testSetLocation();
  e = e || testLink();


  if(e > 0)
    printf("   FAIL!\n");
  else
    printf("   OK!\n");

  return 0;
}