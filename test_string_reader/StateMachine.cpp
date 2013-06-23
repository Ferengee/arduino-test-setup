#include <StateMachine.h>


  
ReaderState * ReaderStartState::process(StreamWrapper * in, CharBufferManager * out){
  ReaderState * result = this;
  int nextToken;
  
  while (result == this){
    nextToken = in->peek();
    if(nextToken == -1){
      in->println("|");
      in->read();
      break;
    }
    switch(nextToken){
      case '"':
        in->read();
        result = normalState;
        break;
      case ' ':
        in->read();
        break;
      default:
        result = errorState;
    }
  }
  return result;
}

ReaderState * ReaderNormalState::process(StreamWrapper * in, CharBufferManager * out){
  ReaderState * result = this;
  char nextToken = in->peek();
  switch(nextToken){
    case '"':
      in->read();
      result = endState;
      break;
    case '\\':
      in->read();
      result = escapedState;
      break;
    default:
      out->putchar(nextToken);
      in->read();
  }
  return result;
}


ReaderState * ReaderEscapedState::process(StreamWrapper * in, CharBufferManager * out){
  ReaderState * result = normalState;
  char nextToken =  in->read();
  switch(nextToken){
    case 'a':
      out->putchar('\a');
      break;
    case 'b':
      out->putchar('\b');
      break;    
    case 'f':
      out->putchar('\f');
      break;
    case 'n':
      out->putchar('\n');
      break;
    case 'r':
      out->putchar('\r');
      break;
    case 't':
      out->putchar('\t');
      break;
    default:
      out->putchar(nextToken);
  }
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
  
  normal.errorState = &error;
  normal.escapedState = &escaped;
  normal.endState = &endstate;
  
  escaped.normalState = &normal;
  
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