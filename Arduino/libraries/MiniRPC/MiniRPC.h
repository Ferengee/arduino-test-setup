#ifndef MiniRPC_h
#define MiniRPC_h
#define MAX_METHOD_COUNT 16

#include <Arduino.h>
#include "StreamWrapper.h"
#include "../../../test_string_reader/BufferManager.h"
#include "StreamParser.h"

class MiniRPCDispatcher;

class MiniRPCMethod
{
public:
  MiniRPCDispatcher * dispatcher;
  /* returns true if finished getting */
  bool get(char * buffer, int len);
  bool get(int arg);
  char * getName();
  void setName(const char * name){ _name = name;}
  virtual void execute(){}
  virtual bool prepare(){return true;}
private:
  const char * _name;
  
  
};

class TestMethod : public MiniRPCMethod
{
  TestMethod(){
    setName("test.method");
  }
  virtual bool prepare(){
    get(arg1, 10);
    get(arg2);
    return true;
  }
  virtual void execute(){
    Serial.print(getName());
    Serial.print(":");
    Serial.println(arg1);
    Serial.println(arg2);
    
  }
private:
  char arg1[10];
  int arg2;
};

class MiniRPCDispatcher
{
public:
  /* returns true on succes */
  bool registerMethod(MiniRPCMethod * method);
private:
  MiniRPCMethod * _methods[MAX_METHOD_COUNT];
  int _methodsCount;
  
  StreamWrapper * streamWrapper;
  CharBufferManager charBufferManager;
  IntBufferManager intBufferManager;
  FloatBufferManager floatBufferManager;
  StringStreamParser stringParser;
  
  
};


#endif