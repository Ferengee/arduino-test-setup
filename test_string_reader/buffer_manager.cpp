#include <buffer_manager.h>

bool CharBufferManager::putchar(char item){
  if (sizeLeft() > 0){
    *_cursor = item; 
    _cursor++;
    _used++;
    
    return true;
  }
  return false;
}

void CharBufferManager::init(char * buffer, int len){
  _buffer = buffer; 
  _cursor = buffer;
  _capacity = len;
  _used = 0;
}


void CharBufferManager::terminateStr(){
  if (sizeLeft() > 0)
    *_cursor = 0; 
  else
    _buffer[_capacity -1] = 0;
}
