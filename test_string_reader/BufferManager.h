#ifndef CharBufferManager_h
#define CharBufferManager_h


class BufferManager
{
public:
  BufferManager(){}
  virtual bool putchar(char item){ return false;}
};

class CharBufferManager : public BufferManager
{
public:
  int sizeLeft(){ return _capacity - _used; }
  virtual bool putchar(char item);
  
  void init(char * buffer, int len);
  char * getBuffer(){ return _buffer; }
  
  void terminateStr();
  
private:
  int _capacity;
  int _used;
  char * _cursor;
  char * _buffer;
};

class IntBufferManager : public BufferManager
{
public:
  virtual bool putchar(char item);
  
  void init();
  int getBuffer(){ return (int)_buffer; }
  
protected:

  long _buffer;
  int _sign;
};

class FloatBufferManager : public IntBufferManager
{
public:
  virtual bool putchar(char item);
  
  void init();
  float getBuffer();
  
private:
  float _fraction;
  bool _isFraction;
};


#endif