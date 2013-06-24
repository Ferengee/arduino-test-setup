#include "StateMachine.h"

ReaderState * ReaderStartState::process(StreamWrapper * in, BufferManager * out){
  ReaderState * result = this;
  int nextToken;
  
  while (result == this){
    nextToken = in->peek();
    if(nextToken == -1){
      in->read();
      break;
    }
    if(nextToken == (int)startToken){
      in->read();
      result = normalState;  
    }else if(nextToken == (int)' '){
      in->read();
    }else if(startToken == 0){
      result = normalState;
    }else{
      result = errorState;
    }
  }
  return result;
}

ReaderState * ReaderNormalState::process(StreamWrapper * in, BufferManager * out){
  ReaderState * result = this;
  int nextToken;
  
  while (result == this){
    nextToken = in->read();
    if(nextToken == -1)
      break;
    
    if(nextToken == (int)endToken){
      result = endState;
    }else if(nextToken == (int) '\\'){
      result = escapedState;
    }else{
      if(!out->putchar(nextToken))
        result = errorState;
    }
  }
  return result;
}


ReaderState * ReaderEscapedState::process(StreamWrapper * in, BufferManager * out){
  ReaderState * result = normalState;
  int nextToken = in->read();
  if(nextToken == -1)
    return this;
  bool writeSucces = false;
  switch(nextToken){
    case 'a':
      writeSucces = out->putchar('\a');
      break;
    case 'b':
      writeSucces = out->putchar('\b');
      break;    
    case 'f':
      writeSucces = out->putchar('\f');
      break;
    case 'n':
      writeSucces = out->putchar('\n');
      break;
    case 'r':
      writeSucces = out->putchar('\r');
      break;
    case 't':
      writeSucces = out->putchar('\t');
      break;
    default:
      writeSucces = out->putchar(nextToken);
    
  }
  if (!writeSucces)
    result = errorState;
  return result;
}

