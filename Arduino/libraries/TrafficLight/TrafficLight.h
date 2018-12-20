
#ifndef TrafficLight_h
#define TrafficLight_h

#include <Arduino.h>

enum TrafficLightColor {RED, GREEN, YELLOW, OFF};
  
class TrafficLight
{
  const int pin1;
  const int pin2;
  const int pin3;
  TrafficLightColor current;
public:
  TrafficLight(const int pin1, const int pin2, const int pin3):pin1(pin1),pin2(pin2),pin3(pin3) {
    init();
    off();
  }

  void on(TrafficLightColor color){
    current = color;
    
    switch(color) {
      case RED : 
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, LOW);
        break;
      
      case YELLOW : 
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, LOW);
        break;
      
      case GREEN :
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, HIGH);
        break;
        
      case OFF:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);  
        break;
    }
  }
  
  void off(){
    on(OFF);
  }
  
  bool isOn(){
    return current != OFF;
  }
  
  
  void init(){
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
  };
};

#endif
