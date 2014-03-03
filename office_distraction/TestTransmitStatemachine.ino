#include "Schedulers.h"
#include "EventChannel.h"

#define POWER_PIN 4
#define LED_PIN 13
#define SILENCE_COUNT 4
#define RESEND_TIMEOUT 400
#define RESTART_TIMEOUT 1000
#define LOSE_TIMEOUT 1000
#define SHUTDOWN_TIMEOUT 500

#define USEMAIN
/*
 * Read the Statemachines tab to see the state machine configuration
 * 
 */
#include "Arduino.h"
void lifecycleTimeOut(void * data);
void initLifeCycle(int token, void * data);
void initShutdownTimer(int token, void * data);
void powerOff(int token, void * data);
void emitSilence(void * data);
void initSilenceCounter(int token, void * data);
void emitRetry(void * data);
void initResendTimer(int token, void * data);
void initRestartTimer(int token, void * data);
void setup();
void loop();
void setup_machines();
enum TransitionEvents {WIN, LOSE, LIFECYCLETIMEOUT, SILENCE, RETRY, ACKNOWLEDGE};


typedef struct {
  Scheduler timer;
  EventChannel * channel;
} lifecycle_context_t;

typedef struct {
  CountdownTimer silenceCounter;
  Scheduler resendTimer;
  Scheduler restartTimer;
  EventChannel * channel;

} messagecycle_context_t;

Schedulers schedulers;
EventChannel eventChannel = EventChannel();

void lifecycleTimeOut(void * data){
  ((lifecycle_context_t *)data)->channel->send(LIFECYCLETIMEOUT, data);  
}
void initLifeCycle(int token, void * data){
    Serial.println("initLifeCycle");

  lifecycle_context_t * context = (lifecycle_context_t *)data;
  context->timer.once(LOSE_TIMEOUT, lifecycleTimeOut, data);
  
}


void initShutdownTimer(int token, void * data){
    Serial.println("initShutdownTimer");
  if(token == WIN){
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  ((lifecycle_context_t *)data)->timer.once(SHUTDOWN_TIMEOUT, lifecycleTimeOut, data);
}
void powerOff(int token, void * data){
  Serial.println("powerOff");

  digitalWrite(POWER_PIN, LOW);
}

void emitSilence(void * data){
  ((messagecycle_context_t *)data)->channel->send(SILENCE, data);  
}
void initSilenceCounter(int token, void * data){
  //TODO configure a reset for the silence test
  //((messagecycle_context_t *)data)->senderReciever
  
  ((messagecycle_context_t *)data)->silenceCounter.start(SILENCE_COUNT, emitSilence, data);
}

void emitRetry(void * data){
  ((messagecycle_context_t *)data)->channel->send(RETRY, data);  
}

void initResendTimer(int token, void * data){
  ((messagecycle_context_t *)data)->resendTimer.once(RESEND_TIMEOUT, emitRetry, data);

}
void initRestartTimer(int token, void * data){
  ((messagecycle_context_t *)data)->restartTimer.once(RESTART_TIMEOUT, emitRetry, data);
}

messagecycle_context_t messagecycleContext;
lifecycle_context_t lifecycleContext;

void setup(){
  //digitalWrite(POWER_PIN, HIGH);
  messagecycleContext.channel = &eventChannel;
  lifecycleContext.channel = &eventChannel;
  schedulers.attach(messagecycleContext.resendTimer);
  schedulers.attach(messagecycleContext.restartTimer);
  
  schedulers.attach(lifecycleContext.timer);

  //eventChannel.send(SILENCE, NULL);

  setup_machines(); 
  
}
void loop(){
  schedulers.trigger();
  delay(10);
}

#include <SimpleStateMachine.h>
#include <EventChannel.h>



State starting, won, lost, done, waitForSilence, sendMessage, wait;

Vertex state_machine_links[9];

Machine lifeCycle = Machine(starting);
Machine messageCycle = Machine(waitForSilence);


void setup_machines(){
  Vertex * l = state_machine_links;

/*  life cycle */
  starting.on(l++, WIN)->to(won);
  starting.on(l++, LOSE)->to(lost);
  starting.on(l++, LIFECYCLETIMEOUT)->to(lost);

  won.on(l++, LIFECYCLETIMEOUT)->to(done);
  lost.on(l++, LIFECYCLETIMEOUT)->to(done);

/*  message cycle */
  waitForSilence.on(l++, SILENCE)->to(sendMessage);
  sendMessage.on(l++, RETRY)->to(sendMessage);
  sendMessage.on(l++, ACKNOWLEDGE)->to(wait);
  wait.on(l++, RETRY)->to(waitForSilence);

/*  life cycle functions */
  starting.enterfunc = initLifeCycle;
  lost.enterfunc = initShutdownTimer;
  won.enterfunc = initShutdownTimer;
  done.enterfunc = powerOff;

/*  message cycle functions */
  waitForSilence.enterfunc = initSilenceCounter;
  sendMessage.enterfunc = initResendTimer;
  wait.enterfunc = initRestartTimer;

/*  machines */
  eventChannel.addListener(lifeCycle);
  eventChannel.addListener(messageCycle);

  lifeCycle.start(&lifecycleContext);
  messageCycle.start(&messagecycleContext);
}


