#include <buffer_manager.h>

int main(void){

  const char * input = "Een aap die geen bananen heet";
  char buffer[10];
  int i = 0;
  
  CharBufferManager m1 = CharBufferManager();
  
  m1.init((char *)&buffer, 10);
  
  for(i=0; i < 10; i++){
    m1.putchar(input[i]);
  }
  
  m1.terminateStr();
  
  std::cout << m1.getBuffer();
  std:: cout << "\n";
  
  return 0; 
}