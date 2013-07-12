#ifndef MiniRPC_h
#define MiniRPC_h
#define MAX_METHOD_COUNT 16
#define MAX_METHOD_NAME_LENGTH 20
#define MINIRPC_ARGUMENT_ERROR -2
#define MINIRPC_ARGUMENT_SKIP -1
#define MINIRPC_ARGUMENT_INIT 0
#define MINIRPC_ARGUMENT_WORKING 1
#define MINIRPC_ARGUMENT_FINISHED 2
#define MINIRPC_ARGUMENT_USAGE 3

#include <Arduino.h>
#include "StreamWrapper.h"
#include "StreamParser.h"
#include "BufferManager.h"

class MiniRPCDispatcher;

class MiniRPCMethod
{
public:
  MiniRPCMethod();
  
  virtual void prepare(){}
  virtual void execute(){}
  virtual void error();
  
  MiniRPCDispatcher * dispatcher;

  /* returns true if finished getting */
  bool get(char * buffer, int len);
  bool get(int &arg);
  bool get(float &arg);
  const char * getName(){ return _name; }
  void setName(const char * name){ _name = name;}

  void pre_prepare();
  //TODO: consume remaining ) if there were no arguments
  void post_prepare(){total_argument_count = current_argument_index + 1 ;}

  bool isReady();
  bool hasError(){return active_argument_state == MINIRPC_ARGUMENT_ERROR;}
  virtual void init(MiniRPCDispatcher * dispatcher);

protected:
  int getStr(char* buffer, int len);
  int getCleanup(int progress);

private:
  const char * _name;
  // the argument (get call) which is currently beeing evaluated
  int current_argument_index;
  // the argument which is allowed to read the stream to become complete
  int active_argument_state; // 0 => init, 1 => working, 2 => finished, -2 => error, -1 => skip
  int active_argument_index;
  int total_argument_count;
  int getStage();
  int stripTerminator();
  
};

class MethodMatcher : public MiniRPCMethod
{
public:
  virtual void execute();
  virtual void prepare();
  virtual void init(MiniRPCDispatcher * dispatcher);
  MethodMatcher();
protected:
  char _method_name[MAX_METHOD_NAME_LENGTH];
  int _method_name_length;
  bool getMethodName(char * _method_name, int len);

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
  /*
   * list must be at least MAX_METHOD_COUNT long
   */
  void getMethodList(MiniRPCMethod ** list, int &length){
    int i;
    for (i=0; i < _methodsCount; i++)
       list[i] = _methods[i];
    length = _methodsCount;
  }
  
  StreamWrapper * streamWrapper;
  
  CharBufferManager charBufferManager;
  StringStreamParser stringStreamParser;
  
  IntBufferManager intBufferManager;
  IntStreamParser intStreamParser;
  
  FloatBufferManager floatBufferManager;
  FloatStreamParser floatStreamParser;
protected:
  //TODO escape interfering tokens in methodName
  virtual void methodNotFound(const char * methodName){
    streamWrapper->print("{'class':'MethodNotFoundException','message':'failed to find ");
    streamWrapper->print(methodName);
    streamWrapper->println("'}");
  } 
private:
  MiniRPCMethod * _methods[MAX_METHOD_COUNT];
  int _methodsCount;
  MiniRPCMethod * selected_method;
  MethodMatcher methodMatcher;
  void reset();

  char _test_value[30];
};


#endif
