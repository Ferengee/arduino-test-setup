#include <MockEthernet.h>

#include "webserver_example.ino"

int main()
{ 
  setup();
  
  for(;;){
    loop();  
  }
  
}
