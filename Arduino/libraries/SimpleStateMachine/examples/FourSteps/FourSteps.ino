#include <SimpleStateMachine.h>
#include <EventChannel.h>

void hello(int token, void * data){
  Serial.print("hello ");
  Serial.print(token);
  Serial.print("\n");
}

void goodbye(int token, void * data){
  Serial.print("goodbye ");
  Serial.print(token);
  Serial.print("\n");
}

void enterAState(int token, void *data){
  Serial.print("A state ");
  Serial.print(token);
  Serial.print("\n");
}

void enterBState(int token, void *data){
  Serial.print("B state ");
  Serial.print(token);
  Serial.print("\n");
}


State endState;
State startState;
State aState;
State bState;

Vertex links[7];

Machine machine = Machine(startState);

enum TransitionEvents {NEXT, BACK, RETURN, ALTERNATIVE};

void setup(){
  

  //s1.join(&startState, &aState);
  Vertex * l = links;
  startState.on(l++, NEXT)->to(aState);
  aState.on(l++, BACK)->to(startState);
  
  startState.on(l++, ALTERNATIVE)->to(bState);
  bState.on(l++, BACK)->to(startState);
  
  aState.on(l++, NEXT)->to(endState);
  bState.on(l++, NEXT)->to(endState);
  
  endState.on(l++, RETURN)->to(startState);
  
  startState.enterfunc = hello;
  endState.enterfunc = goodbye;
  aState.enterfunc = enterAState;
  bState.enterfunc = enterBState;
}

void loop(){
  
  int steps[] = {NEXT,BACK,ALTERNATIVE,NEXT,BACK,RETURN,ALTERNATIVE,BACK,NEXT,NEXT};
  int i;
  char data[] = "test data";
  for (i=0; i < 10; i++){
    delay(100);
    bool succes = machine.receive(steps[i], (void *)data);
    if (!succes){
      Serial.print("skipped: ");
      Serial.println(steps[i]);
    }
  }
  delay(1000);
}
