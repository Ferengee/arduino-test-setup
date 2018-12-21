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


State listening, maybeGreen, green, maybeRed, red, maybePlayRecorded, autoGreen, maybePlayMorse, playMorse, autoRed, autoYellow;

Machine machine = Machine(listening);
enum TransitionEvents {KNOCK, GO, LISTEN, PROGRESS};

Scheduler knockDoneTimer;
Scheduler blinkTimer;
Scheduler progressTimer;

void emitGo(void * data){
  light.off();
  machine.receive(GO, NULL);
}

void emitProgress(void * data){
  light.off();
  machine.receive(PROGRESS, NULL);
}

void confirmKnock(int token, void * data){
  blinkTimer.stop();
  progressTimer.stop();
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

void lightGreenAndResetTimer(int token, void * data){
  light.on(GREEN);
  progressTimer.once(light.uptimeSecondsGreen *  1000, emitProgress, NULL);
  machine.receive(LISTEN, NULL);
}
void lightYellowAndResetTimer(int token, void * data){
  light.on(YELLOW);
  progressTimer.once(2000, emitProgress, NULL);

  machine.receive(LISTEN, NULL);
}
void lightRedAndResetTimer(int token, void * data){
  light.on(RED);
  progressTimer.once(light.uptimeSecondsRed * 1000, emitProgress, NULL);

  machine.receive(LISTEN, NULL);
}

void lightPlayRecorded(int token, void * data){  
  machine.receive(PROGRESS, NULL);
}

const char sig[] PROGMEM = {"bme"};

void lightPlayMorse(int token, void * data){
  knockSensor.disable();
  speaker.enable();
  
  char base = 'c';
  for(int i =0; i < strlen_P(sig); i++){
    auto c = pgm_read_byte_near(sig + i);
    auto note = c - base;
    speaker.note(note, 4);
    m.send(c);  
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
  progressTimer.trigger();
  delay(10);
}
