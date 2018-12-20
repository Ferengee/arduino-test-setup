#include <Schedulers.h>
#include <SimpleStateMachine.h>
#include <EventChannel.h>
#include <TrafficLight.h>
#include <Piezo.h>
#include <MorseCode.h>

class LightMorseOutput : public MorseOutput {
  PiezoSpeaker * speaker;
  TrafficLight * light;

  MorseOutput * signal(const int duration){
    speaker->unMute();
    light->on(YELLOW);
    delay(duration);
    speaker->mute();
    light->off();
    delay(500);
    return this;
  }
  
public:
  LightMorseOutput(PiezoSpeaker * speaker, TrafficLight * light):speaker(speaker),light(light){}
   virtual MorseOutput * dot(){
     return signal(500);
  }
  
  virtual MorseOutput * dash(){
    return signal(1500);
  }
  
  virtual MorseOutput * space(){
    light->on(RED);
    delay(100);
    light->off();
    delay(500);
    return this;
  }
  
};

KnockSensor knockSensor(PB3, 10, 50);
PiezoSpeaker speaker(4);
TrafficLight light(PB0,PB1,PB2);

LightMorseOutput out(&speaker, &light);
MorseCode m(&out);


State listening, maybeGreen, green, maybeRed, red, maybePlayRecorded, playRecorded;
Machine machine = Machine(listening);
enum TransitionEvents {KNOCK, GO, LISTEN};

Scheduler knockDoneTimer;
Scheduler blinkTimer;



void emitGo(void * data){
  light.off();
  machine.receive(GO, NULL);
}

void confirmKnock(int token, void * data){
  blinkTimer.stop();
  light.on(YELLOW);
  knockDoneTimer.once(600, emitGo, NULL);
}

void lightGreen(int token, void * data){
  light.on(GREEN);
  machine.receive(LISTEN, NULL);
}

void lightRed(int token, void * data){
  light.on(RED);
  machine.receive(LISTEN, NULL);
}

void lightPlayRecorded(int token, void * data){
  knockSensor.disable();
  speaker.enable();
  auto sig = "bme";
  char base = 'c';
  for(int i =0; sig[i] != '\0'; i++){
    auto note = sig[i] - base;
    speaker.note(note, 4);
    m.send(sig[i]);  
  }
  speaker.disable();
  knockSensor.enable();
  
  machine.receive(LISTEN, NULL);
}

void emitListen(void * data){
  machine.receive(LISTEN, NULL);
}

void emitKnock(void * data)
{
  machine.receive(KNOCK, NULL);
}

void blinkYellow(void * data){
  if(light.isOn())
    light.off();
  else
    light.on(YELLOW);
}

void setup(){
  speaker.disable();
  setup_machines();
  knockSensor.callback = emitKnock;
  knockSensor.enable();

  light.init();
  blinkTimer.every(400, blinkYellow, NULL);
}

void loop(){
  knockSensor.sample();
  knockDoneTimer.trigger();
  blinkTimer.trigger();
  delay(10);
}
