#include <SimpleStateMachine.h>
typedef struct {
  bool started;
} process_context_t;

void hello(int token, void * data){
  ((process_context_t *)data)->started = true;; 

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


process_context_t context;

void setup(){
  
  context.started = false;
  
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
  
  machine.start(&context);
}



void loop(){
  
  int steps[] = {NEXT,BACK,ALTERNATIVE,NEXT,BACK,RETURN,ALTERNATIVE,BACK,NEXT,NEXT};
  int i;
  char data[] = "test data";
  if(context.started){
    for (i=0; i < 10; i++){
      delay(100);
      bool succes = machine.receive(steps[i], (void *)data);
      if (!succes){
        Serial.print("skipped: ");
        Serial.println(steps[i]);
      }
    }
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
}
