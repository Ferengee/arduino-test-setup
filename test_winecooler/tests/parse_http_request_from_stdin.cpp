#include "server/server.h"
#include "handler/handler.h"

#include "dbg.h"
#include <DummySerial.h>

int main(){
  ApiRequest request;
  Stream console;
  
  request.setStream(&console);
  
  request.initialize();
  
  int id = request.getInstanceId();
  char * key =  request.getKey();
  printf("Key: (%s) <br>\n", key);
  printf("Instance id: (%d) <br>\n", id);
  printf("...\n");
}