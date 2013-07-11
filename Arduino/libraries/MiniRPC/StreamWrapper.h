#ifndef StreamWrapper_h
#define StreamWrapper_h
#include <Arduino.h>
class StreamWrapper
{
  
public:
  virtual int peek(){ 
    if (_stream->available())
      return _stream->peek();
    else 
      return -1;
  }
  virtual int read(){
    if (_stream->available()){
      int r = _stream->read();
      _stream->print((char)r);
      return r;
    } else {
      return -1;
    }
  }
  void setStream(Stream * stream){_stream = stream;}
  void println(const char* arg) {_stream->println(arg);}
  void print(const char* arg){_stream->print(arg);}
  void print(int arg){_stream->print(arg);}
  void print(float arg){_stream->print(arg);}
private:
  Stream * _stream;
  
};
#endif