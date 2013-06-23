#ifndef CharBufferManager_h
#define CharBufferManager_h

#include <iostream>

class CharBufferManager
{
public:
  int sizeLeft(){ return _capacity - _used; }
  bool putchar(char item);
  
  void init(char * buffer, int len);
  char * getBuffer(){ return _buffer; }
  
  void terminateStr();
  
private:
  int _capacity;
  int _used;
  char * _cursor;
  char * _buffer;
};


#endif