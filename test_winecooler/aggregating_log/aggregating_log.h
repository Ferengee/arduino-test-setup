#ifndef _AGGREGATING_LOG_H
#define _AGGREGATING_LOG_H
 
#include <Arduino.h>
#include <stdint.h>

#define AGGREGATING_LOG_SIZE 3
class AggregatingLog
{
public:
  static void linkChain(AggregatingLog * buffers, int length);
  AggregatingLog();
  void add(int value);
  int getAverage();
  int toJSON(char * out);
  int chainToJSON(char * out);

protected:  
  int values[AGGREGATING_LOG_SIZE];
  uint8_t cursor;
  AggregatingLog * next;
};

class AnalogSampler {
public:
  AnalogSampler(){
    pin = A0;
    log = NULL;
  };
  void sample(){
    if (log != NULL){
      delay(10);
      analogRead(pin);
      delay(10);
      log->add(analogRead(pin));
    }
  };
  AggregatingLog * log;
  int pin;
  
};

#endif