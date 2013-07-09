#include "MiniRPC.h"

MiniRPCDispatcher::MiniRPCDispatcher(StreamWrapper * wrapper)
{
  streamWrapper = wrapper;
  
  stringStreamParser.setStreamWrapper(wrapper);
  stringStreamParser.setBufferManager(&charBufferManager);

  intStreamParser.setStreamWrapper(wrapper);
  intStreamParser.setBufferManager(&intBufferManager);

  floatStreamParser.setStreamWrapper(wrapper);
  floatStreamParser.setBufferManager(&floatBufferManager);
  
  select(&methodMatcher);
  
}

bool MiniRPCDispatcher::registerMethod(MiniRPCMethod* method)
{
  bool succes = _methodsCount < MAX_METHOD_COUNT;
  if (succes){
    _methods[_methodsCount++] = method;
    method->dispatcher = this;
  }
  return succes;
}

void MiniRPCDispatcher::select(char* method_name, int length)
{
  int i;
  bool found = false;
  for (i=0; i < _methodsCount; i++){
    if (strcmp(method_name, _methods[i]->getName()) == 0){
      found = true;
      select(_methods[i]);
      break;
    }  
  }
  if(!found){
    Serial.println("failed to find:");
  
    Serial.println(method_name);
  }
}

void MiniRPCDispatcher::select(MiniRPCMethod* method)
{
  selected_method = method;
  //reset();
  //method->dispatcher = this;
  method->init(this);
  
}

void MiniRPCDispatcher::reset()
{
  stringStreamParser.reset();
  intStreamParser.reset();
  floatStreamParser.reset();

}


bool MiniRPCDispatcher::update()
{
  MiniRPCMethod * active_method = selected_method;
  if(active_method->ready()){
      active_method->execute();
      /*
       * the called method can update the dispatchers state 
       */
      if(active_method == selected_method){
        select(&methodMatcher);
      }
  }else{
    active_method->pre_prepare();
    active_method->prepare();
    if (active_method->error())
    {
      return false;
    }
  }
  return true;
}


bool MiniRPCDispatcher::test(){
int state;
  charBufferManager.init(_test_value,30);
  stringStreamParser.reset();
  while((state = stringStreamParser.process()) == 0){
    Serial.print("_"); 
  }
  if (state == -1){
    Serial.println("Error");
  }else{
    Serial.println("Done");
    charBufferManager.terminateStr();
    Serial.println(charBufferManager.getBuffer());
  }
  return false;
}

MiniRPCMethod::MiniRPCMethod()
{
  active_argument_state = MINIRPC_ARGUMENT_INIT;
  active_argument_index = 0;
  current_argument_index = 0;
}

void MiniRPCMethod::pre_prepare()
{
  current_argument_index = -1;
}

void MiniRPCMethod::init(MiniRPCDispatcher * dispatcher)
{
  active_argument_state = MINIRPC_ARGUMENT_INIT;
  active_argument_index = 0;
  current_argument_index= 0;
  this->dispatcher = dispatcher;
  dispatcher->stringStreamParser.configure('"', '"');
}


int MiniRPCMethod::getStage()
{
  current_argument_index++;
  if(current_argument_index == active_argument_index)
    return active_argument_state;
  else
    return MINIRPC_ARGUMENT_SKIP;
      
}

int MiniRPCMethod::stripTerminator()
{
  StreamWrapper * in = dispatcher->streamWrapper;
  int terminator = 0; 
  int progress = 1;
  terminator = in->read();
  while(terminator == ' ' || terminator == '\t' || terminator == '\n') 
    terminator = in->read();

  if (terminator == -1){
    progress = 0;
  } else if(terminator != ',' && terminator != ')'){  
    active_argument_state = MINIRPC_ARGUMENT_ERROR;
    progress = -1;
  }
  
  return progress;
}

int MiniRPCMethod::getCleanup(int progress)
{
  switch(progress){
    case 1:
      active_argument_index = current_argument_index+1;
      active_argument_state = MINIRPC_ARGUMENT_INIT;
      break;
    case -1:
      active_argument_state = MINIRPC_ARGUMENT_ERROR;
  }
  return progress == 1;

}


bool MiniRPCMethod::get(char* buffer, int len)
{
  int progress = getStr(buffer, len);
  //while(progress == 0){
  //  progress = dispatcher->stringStreamParser.process();
  //}
  if(progress == 1)
    progress = stripTerminator();
  return getCleanup(progress);
}

int MiniRPCMethod::getStr(char* buffer, int len)
{
   int mg = getStage();
  switch(mg){
    case MINIRPC_ARGUMENT_SKIP:
      return false;

    case MINIRPC_ARGUMENT_INIT:
   
      dispatcher->stringStreamParser.reset();
      dispatcher->charBufferManager.init(buffer, len);
      active_argument_state = MINIRPC_ARGUMENT_WORKING;
  }
  
  /*
    * clock the parser unless it is in the end state
    * if the parser is in the end state
    *        update active_argument_index to current_argument_index + 1
    *        set active_argument_state to MINIRPC_ARGUMENT_INIT
    */
  return dispatcher->stringStreamParser.process();
}

bool MethodMatcher::getMethodName(char* method_name, int len)
{
  int progress = getStr(method_name, len);
  return getCleanup(progress);
}

bool MiniRPCMethod::get(int &arg)
{
  int mg = getStage();
  switch(mg){
    case MINIRPC_ARGUMENT_SKIP:
      return false;

    case MINIRPC_ARGUMENT_INIT:
      dispatcher->intStreamParser.reset();
      dispatcher->intBufferManager.init();
      active_argument_state = MINIRPC_ARGUMENT_WORKING;
  }
 

  int progress = dispatcher->intStreamParser.process();
  if(progress == 1){
    arg = dispatcher->intBufferManager.getBuffer();
    progress = stripTerminator();   
  }
  return getCleanup(progress);
}


bool MiniRPCMethod::get(float &arg)
{
  int mg = getStage();
  switch(mg){
    case MINIRPC_ARGUMENT_SKIP:
      return false;

    case MINIRPC_ARGUMENT_INIT:
      dispatcher->floatStreamParser.reset();
      dispatcher->floatBufferManager.init();
      active_argument_state = MINIRPC_ARGUMENT_WORKING;
  }

  int progress = dispatcher->floatStreamParser.process();
  if(progress == 1){
    arg = dispatcher->floatBufferManager.getBuffer();
    progress = stripTerminator();
    
  }
return getCleanup(progress);
}

/*
 * the result of this method is incorrect if it is called inside the branch of a get(...) call
 * The result is only valid after at least one call to prepare()
 * It returns true if the active_argument_state == MINIRPC_ARGUMENT_INIT && active_argument_index == current_argument_index + 1
 * which implies that all the arguments have succesfully been parsed
 */
bool MiniRPCMethod::ready()
{
  return active_argument_state == MINIRPC_ARGUMENT_INIT && active_argument_index == current_argument_index + 1;
}

MethodMatcher::MethodMatcher()
{

}


void MethodMatcher::execute()
{
    Serial.print("calling dispatcher: ");
    Serial.println(dispatcher->charBufferManager.getBuffer());
  dispatcher->select(_method_name, _method_name_length); 
}

void MethodMatcher::prepare()
{
  if(getMethodName(_method_name, MAX_METHOD_NAME_LENGTH))
  {
    _method_name_length = dispatcher->charBufferManager.used();
    dispatcher->charBufferManager.terminateStr();    
  }
  
}

void MethodMatcher::init(MiniRPCDispatcher * dispatcher)
{
    MiniRPCMethod::init(dispatcher);
    dispatcher->stringStreamParser.configure(0, '(');
}

