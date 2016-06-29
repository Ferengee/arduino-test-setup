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
/*
#include <SPI.h>
#include <Ethernet.h>
*/

#include <Schedulers.h>
#include "lib.c"
#include "server/server.h"


#define LINK_COUNT 3

Scheduler sampler;

Schedulers schedulers;

AggregatingLog temp[LINK_COUNT];
//AggregatingLog pressure[LINK_COUNT];

AnalogSampler tempSensor;
//AnalogSampler pressureSensor;



char output[40];

void printChain(){
  
  Serial.println("temp:" );

  temp->chainToJSON(output);
  Serial.println(output);

  //Serial.println("pressure:" );
  //pressure->chainToJSON(output);

  //Serial.println(output);
}


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
  
 printChain();
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
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  AggregatingLog::linkChain(temp, LINK_COUNT);
  //AggregatingLog::linkChain(pressure, LINK_COUNT);
  
  tempSensor.pin = A0;
  //pressureSensor.pin = A1;
  
  tempSensor.log = temp;
  //pressureSensor.log = pressure;

  sampler.every(500, takeSample, NULL);
  
  schedulers.attach(sampler);
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200/OK");
          client.println("Access-Control-Allow-Origin: * ");
          client.println("Content-Type: text/plain");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.print("{\"temp\":");
          temp->chainToJSON(output);
          client.print(output);
          client.print("}");
         
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    
    // pause for testing
  } 
      delay(500);

    
    schedulers.trigger();

}


