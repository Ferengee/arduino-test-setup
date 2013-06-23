#include <buffer_manager.h>
#include <DummySerial.h>
class ReaderState
{
public:
  virtual ReaderState * process(Stream * in, CharBufferManager * out){ return this;}
  bool done() {return _done;}

private:
  bool _done;
  
};

class ReaderStartState : public ReaderState
{
public:
  
  virtual ReaderState * process(Stream * in, CharBufferManager * out){
    ReaderState * result = this;
    char nextToken = in->peek();
    switch(nextToken){
      case '"':
        in->read();
        result = _ReaderNormalState;
        break;
      case ' ':
        in->read();
        break;
      default:
        result = _ReaderErrorState;
    }
    return result;
  }
private:
  ReaderState * _ReaderErrorState;
  ReaderState * _ReaderNormalState;
  
};

class ReaderEndState : public ReaderState
{

};

class ReaderEscapedState : public ReaderState
{

};

int main(void)
{
  char input[] = "\"een \"aap\" die geen bananen eet\"";
  char output[40];
  
  ReaderState reader = ReaderStartState();
  return 0;
}