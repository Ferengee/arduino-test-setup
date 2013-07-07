#ifndef MiniRPC_h
#define MiniRPC_h
#define MAX_METHOD_COUNT 16
#define MAX_METHOD_NAME_LENGTH 20
#define MINIRPC_ARGUMENT_ERROR -2
#define MINIRPC_ARGUMENT_SKIP -1
#define MINIRPC_ARGUMENT_INIT 0
#define MINIRPC_ARGUMENT_WORKING 1
#define MINIRPC_ARGUMENT_FINISHED 2

#include <Arduino.h>
#include "StreamWrapper.h"
#include "StreamParser.h"
#include "BufferManager.h"

class MiniRPCDispatcher;

class MiniRPCMethod
{
public:
  MiniRPCDispatcher * dispatcher;
  MiniRPCMethod();
  /* returns true if finished getting */
  bool get(char * buffer, int len);
  bool get(int arg);
  bool get(float arg);
  const char * getName(){ return _name; }
  void setName(const char * name){ _name = name;}
  virtual void execute(){}
  virtual void prepare(){}
  void pre_prepare();
  bool ready();
  bool error(){return active_argument_state == MINIRPC_ARGUMENT_ERROR;}
  void init();
private:
  const char * _name;
  // the argument (get call) which is currently beeing evaluated
  int current_argument_index;
  // the argument which is allowed to read the stream to become complete
  int active_argument_state; // 0 => init, 1 => working, 2 => finished, -2 => error, -1 => skip
  int active_argument_index;
  int getStage();
  
};

class TestMethod : public MiniRPCMethod
{
public:
  TestMethod(){
    setName("test.method");
  }
  virtual void prepare(){
    get(arg1, 10);
    get(arg2);
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


class MethodMatcher : public MiniRPCMethod
{
public:
  virtual void execute();
  virtual void prepare();
  MethodMatcher();
protected:
  char _method_name[MAX_METHOD_NAME_LENGTH];
  int _method_name_length;
};

class MiniRPCDispatcher
{
public:
  MiniRPCDispatcher(StreamWrapper* wrapper);
  MiniRPCDispatcher(){}
  /* returns true on succes */
  bool registerMethod(MiniRPCMethod * method);
  /*
   * call prepare on the current active method
   * if it is done preparing, execute it
   * after execution select the correct next active method
   */
  bool update();
  void select(char * method_name, int length);
  void select (MiniRPCMethod * method);
  bool test();
  
  StreamWrapper * streamWrapper;
  
  CharBufferManager charBufferManager;
  StringStreamParser stringStreamParser;
  
  IntBufferManager intBufferManager;
  IntStreamParser intStreamParser;
  
  FloatBufferManager floatBufferManager;
  FloatStreamParser floatStreamParser;
private:
  MiniRPCMethod * _methods[MAX_METHOD_COUNT];
  int _methodsCount;
  MiniRPCMethod * selected_method;
  MethodMatcher methodMatcher;
  void reset();
  char _test_value[30];
};


#endif