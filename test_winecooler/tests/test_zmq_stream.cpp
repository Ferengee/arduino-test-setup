#include "dbg.h"
#include <ZmqStream.h>

int main(){
  ZmqStream tcp;
  
  zmq::context_t context(1);

  zmq::socket_t responder(context, ZMQ_REP);
  responder.bind("tcp://*:5559");

  tcp.setSocket(&responder);
    
  /*request.setStream(&tcp);
  
  request.initialize();
  
  int id = request.getInstanceId();
  char * key =  request.getKey();
  printf("Key: (%s) <br>\n", key);
  printf("Instance id: (%d) <br>\n", id);
  printf("...\n");
  */
  
  while(1){
    while(tcp.available()){
      char c = 0;

      while((c = tcp.read()) > -1){
        printf("%c",c);
      }
      
    sleep(1);

    }
    printf("\ntcp not available\n");

    
  }
      sleep(1);

}
