#ifndef StreamWrapper_h
#define StreamWrapper_h
#include <Arduino.h>
class StreamWrapper
{
  
public:
  virtual int peek(){return _stream->peek();}
  virtual int read(){return _stream->read();}
  void setStream(Stream * stream){_stream = stream;}
private:
  Stream * _stream;
  
};
#endif