#include "StateMachine.h"

ReaderState * ReaderStartState::process(StreamWrapper * in, BufferManager * out){
  ReaderState * result = this;
  int nextToken;
  
  while (result == this){
    nextToken = in->peek();
    if(nextToken == -1){
      result = this;
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
    }else {
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



/*
 * x E {'+', '-'} => signedState, out.sign = x == - ? -1 : 1
 * x E {'0'..'9'} => valueState, out.putchar(x)
 * x E {'.'} and fractionState => fractionState, out.isFraction = true
 * else => errorState
 * 
 */
ReaderState* ReaderStartNumericState::process(StreamWrapper* in, BufferManager* out)
{
  ReaderState * result = this;
  int nextToken;

  while (result == this){
    nextToken = in->peek();
    if(nextToken == -1){
      result = this;
      in->read();
      break;
    }
    if(nextToken == '+' ||  nextToken == '-'){
      result = signedState;
      out->putchar(nextToken);
      in->read();
    }else if(out->isNumeric(nextToken)){
      result = valueState;
      out->putchar(nextToken);
      in->read();
    }else if(nextToken == '.' && fractionState != NULL){
      result = fractionState;
      out->putchar(nextToken);
      in->read();
    }else {
      result = errorState; 
    }
  }
  return result;
}


/*
 * x E {'0'..'9'} => this, out.putchar(x)
 * x E {'.'} and fractionState => fractionState, out.isFraction = true
 * elseif fractionState == null => errorState
 * else => endState
 * 
 */
ReaderState* ReaderValueState::process(StreamWrapper* in, BufferManager* out)
{
  ReaderState * result = this;
  int nextToken;

  while (result == this){
    nextToken = in->peek();
    if(nextToken == -1){
      result = this;
      in->read();
      break;
    }
    if(out->isNumeric(nextToken)){
      result = this;
      out->putchar(nextToken);
      in->read();
    }else if(nextToken == '.' && fractionState != NULL){
      result = fractionState;
      out->putchar(nextToken); 
      in->read();
    }else {
      result = endState; 
    }
  }
  return result;
}

/*
 *  x E {'0'..'9'} => this, out.putchar(x)
 *  else => endState
 */

ReaderState* ReaderFractionState::process(StreamWrapper* in, BufferManager* out)
{
  ReaderState * result = this;
  int nextToken;

  while (result == this){
    nextToken = in->peek();
    if(nextToken == -1){
      result = this;
      in->read();
      break;
    }
    if(out->isNumeric(nextToken)){
      result = this;
      out->putchar(nextToken);
      in->read();
    }else {
      result = endState; 
    }
  }
  return result;
  
}

/*
 * x E {'0'..'9'} => valueState, out.putchar(x)
 * x E {'.'} and fractionState => fractionState, out.isFraction = true
 * 
 */

ReaderState* ReaderSignedState::process(StreamWrapper* in, BufferManager* out)
{
    return ReaderState::process(in, out);
}

