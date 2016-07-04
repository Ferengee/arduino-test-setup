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
#include "aggregating_log/aggregating_log.h"
#include "server/server.h"
#include "handler/handler.h"

#define LINK_COUNT 3
#define LOG_COUNT 2

class SensorHandler : public AbstractHandler {
public:
  SensorHandler(){}
  
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
  
  AggregatingLog logs[LOG_COUNT][LINK_COUNT];
};


Scheduler sampler;

Schedulers schedulers;

//AggregatingLog pressure[LINK_COUNT];

AnalogSampler tempSensor;
//AnalogSampler pressureSensor;


SensorHandler sensors;


void takeSample(void * nothing){
  /*analogRead(A0);
  delay(10);
  temp->add(analogRead(A0));
  delay(10);
  
  analogRead(A1);
  delay(10);
  pressure->add(analogRead(A1));
  delay(10);
*/
  tempSensor.sample();
 //pressureSensor.sample();
}
  

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
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  
  server.on("sensors")->use(&sensors);
  
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  AggregatingLog::linkChain(sensors.logs[0], LINK_COUNT);
  //AggregatingLog::linkChain(pressure, LINK_COUNT);
  
  tempSensor.pin = A0;
  //pressureSensor.pin = A1;
  
  tempSensor.log = sensors.logs[0];
  //pressureSensor.log = pressure;

  sampler.every(500, takeSample, NULL);
  
  schedulers.attach(sampler);
}


void loop() {
  server.handleIncommingRequests();
    
   /*
    *   int value = 17;
            // send a standard http response header
      client.println("HTTP/1.1 200/OK");
      client.println("Access-Control-Allow-Origin: * ");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");  // the connection will be closed after completion of the response
      client.println();
      client.print("{\"temp\":");
        temp->chainToJSON(output);
      client.print(output);
      client.print(",\"key\":\"");
      client.print(request.getKey());
      client.print("\",\"id\":");
      client.print(request.getInstanceId());
      
      client.print(",\"value\":");

      if (request.method() != GET)
        value = request.intData();
      
      client.print(value);
      client.println("}");
    
    // pause for testing
  } 
     */ 
    delay(500);

    
    schedulers.trigger();

}


