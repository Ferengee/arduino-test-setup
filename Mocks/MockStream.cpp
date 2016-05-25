#include "MockStream.h"

void MockStream::setSourceString(char * string, int len){
  head = 0;
  max = len;
  sourceString = string;
}

int MockStream::read(void){
  if (head >= max)
    return -1;
  return sourceString[head++];
}

int MockStream::unread(char c){
  if (head >= max)
    return -1;
  
  sourceString[--head] = c;
  return 1;
}


