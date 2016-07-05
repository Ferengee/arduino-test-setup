/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include <SPI.h>
#include <Ethernet.h>

#include <Schedulers.h>
#include <Relay.h>

#include "aggregating_log/aggregating_log.h"
#include "server/server.h"
#include "handler/handler.h"

#define LINK_COUNT 3
#define LOG_COUNT 4
#define VARIABLE_COUNT 4

class VariableHandler: public AbstractHandler {
public:
  virtual bool handle(ApiRequest * request){
    uint8_t id = request->getInstanceId();
    
    if(id > 0 && id <= VARIABLE_COUNT){
      
      if (request->method() == POST){
        variables[id -1] = request->intData();
      }
      
      request->sendJsonHeaders();
      request->respond(variables[id -1]);

      return true;
    }
    return false;

  }
  
  int variables[VARIABLE_COUNT];
  
};

class SensorHandler : public AbstractHandler {
public:
  SensorHandler(){
    int i =0;
    
    for (i=0; i < LOG_COUNT; i++){
      samplers[i].log = logs[i];
      AggregatingLog::linkChain(logs[i], LINK_COUNT);
      samplers[i].pin = A0 + i;
    }
  }
  
  virtual bool handle(ApiRequest * request){
    uint8_t id = request->getInstanceId();
    char output[44];
    if(id > 0 && id <= LOG_COUNT){
      logs[id -1]->chainToJSON(output);
      request->sendJsonHeaders();
      request->respond(output);
      return true;
    }
    return false;
  }
  
  void sample(){
    int i =0;
    for (i=0; i < LOG_COUNT; i++){
      samplers[i].sample();
    }
  }
  
  AggregatingLog logs[LOG_COUNT][LINK_COUNT];
  AnalogSampler samplers[LOG_COUNT];

};


Scheduler sampler;

Schedulers schedulers;



SensorHandler sensors;
VariableHandler variables;

void takeSample(void * nothing){
  sensors.sample();
}

Relay cooler = Relay(3);

  

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
ApiServer server;


void setup() {
  pinMode(A0, INPUT_PULLUP );
  pinMode(A1, INPUT_PULLUP );
  pinMode(A2, INPUT_PULLUP );
  pinMode(A3, INPUT_PULLUP );

  Ethernet.begin(mac, ip);
  
  server.on("sensor")->use(&sensors);
  server.on("variable")->use(&variables);

  
  server.begin();

  sampler.every(500, takeSample, NULL);
  
  schedulers.attach(sampler);
}


void loop() {
  server.handleIncommingRequests();
    
  delay(500);
  
  int temp_a = sensors.logs[0]->getAverage();
  int temp_b = sensors.logs[1]->getAverage();
  
  printf("temp_a: %d, temp_b: %d\n", temp_a, temp_b);
  printf("var_a: %d, var_b: %d\n", variables.variables[0], variables.variables[1]);
  
  
  if(min(temp_a, temp_b) < variables.variables[0]) {
    cooler.off();
  }
  
  if(max(temp_a, temp_b) > variables.variables[1]) {
    cooler.on();
  }

  schedulers.trigger();

}


