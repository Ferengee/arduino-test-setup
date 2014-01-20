#include <SimpleStateMachine.h>

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

Machine machine = Machine(&startState);

enum TransitionEvents {NEXT, BACK, RETURN, ALTERNATIVE};

void setup(){
  

  //s1.join(&startState, &aState);
  Vertex * n = links;
  startState.on(n++, NEXT)->to(&aState);
  aState.on(n++, BACK)->to(&startState);
  
  startState.on(n++, ALTERNATIVE)->to(&bState);
  bState.on(n++, BACK)->to(&startState);
  
  aState.on(n++, NEXT)->to(&endState);
  bState.on(n++, NEXT)->to(&endState);
  endState.on(n++, RETURN)->to(&startState);
  
  startState.enterfunc = hello;
  endState.enterfunc =goodbye;
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
    if (!succes)
      cout << "skipped: " << steps[i] << "\n";
  }
  delay(1000);
}

int main()
{ 
  setup();
  unsigned long start = millis();
  unsigned long t = 0;
  while(t < 10000){
    loop();
    t = millis() - start;
  }
  //endwin();

}
