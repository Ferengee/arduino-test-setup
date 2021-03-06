#include "BufferManager.h"

bool CharBufferManager::write(char item){
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

/* no spaces allowed between sign and value */
bool IntBufferManager::write(char item){
  
  if(_sign == 0){
    if(item == '-'){
      _sign = -1;
      return true;
    }else{
      _sign = 1;
      if(item == '+')
        return true;
    }
  }
  if (isNumeric(item)){
    _buffer = _buffer * 10 + (item - '0') * _sign;  
  
    return true;
  }
  return false;
}

void IntBufferManager::init(){
  _buffer = 0;
  _sign = 0;
}

bool FloatBufferManager::write(char item){
  if(item == '.'){
    if(_isFraction){
      return false;
    }else{
      _isFraction = true;
      return true; 
    }
  }
    
  if(_isFraction)
    _fraction *= 0.1;
  return IntBufferManager::write(item);
 
}

void FloatBufferManager::init(){
  IntBufferManager::init();
  _fraction = 1.0;
  _isFraction = false;
}

float FloatBufferManager::getBuffer(){
  return _buffer * _fraction;
}