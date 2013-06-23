#include <StateMachine.h>


  
ReaderState * ReaderStartState::process(StreamWrapper * in, CharBufferManager * out){
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

ReaderState * ReaderNormalState::process(StreamWrapper * in, CharBufferManager * out){
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


ReaderState * ReaderEscapedState::process(StreamWrapper * in, CharBufferManager * out){
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


int main(void)
{
  char output[40];
  
  StreamWrapper in = StreamWrapper();
  
  CharBufferManager bm = CharBufferManager();
  bm.init(output, 40);
  
  ReaderStartState reader = ReaderStartState();
  ReaderState error = ReaderEndState();
  ReaderState endstate = ReaderEndState();

  ReaderNormalState normal = ReaderNormalState();
  ReaderEscapedState escaped = ReaderEscapedState();
  
  ReaderState * nextState;
  
  reader.errorState = &error;
  reader.normalState = &normal;
  reader.startToken = 0;
  
  normal.errorState = &error;
  normal.escapedState = &escaped;
  normal.endState = &endstate;
  normal.endToken = '(';
  
  escaped.normalState = &normal;
  escaped.errorState = &error;

  
  nextState = &reader;
  while (nextState != &error){
    nextState = nextState->process(&in, &bm);
    if(nextState == &escaped)
      Serial.print("'");
    else if (nextState == &normal)
      Serial.print("_");
    else
      Serial.print(".");
    if(nextState == &endstate)
      break;
  }
  if(nextState == &error){
    Serial.println(" Error");
  }else{
    Serial.println(" Done");
  }
  bm.terminateStr();
  Serial.println(bm.getBuffer());
  return 0;
}