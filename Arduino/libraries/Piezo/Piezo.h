
#ifndef Piezo_h

#include <Arduino.h>
#include <Schedulers.h>

const uint8_t scale[] PROGMEM = {239,226,213,201,190,179,169,160,151,142,134,127};

class PiezoSpeaker 
{
  const int pin;                                   // Can be 1 or 4
  // Cater for 16MHz, 8MHz, or 1MHz clock:
  const int Clock = ((F_CPU/1000000UL) == 16) ? 4 : ((F_CPU/1000000UL) == 8) ? 3 : 0;
  
  int currentNote;
  int currentOctave;
  
public:
  PiezoSpeaker(const int pin):pin(pin){}
  
  void enable(){
    pinMode(pin, OUTPUT);
  }

  void disable(){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  void note (int n, int octave) {
    if (octave != 0){
      currentNote = n;
      currentOctave = octave;
    }
    
    int prescaler = 8 + Clock - (octave + n/12);
    if (prescaler<1 || prescaler>15 || octave==0) prescaler = 0;
    DDRB = (DDRB & ~(1<<pin)) | (prescaler != 0)<<pin;
    OCR1C = pgm_read_byte(&scale[n % 12]) - 1;
    GTCCR = (pin == 4)<<COM1B0;
    TCCR1 = 1<<CTC1 | (pin == 1)<<COM1A0 | prescaler<<CS10;
  }

  void mute (){
    note(0,0);
  }
  
  void unMute(){
    note(currentNote, currentOctave);
  }
};

class KnockSensor
{
  const int pin;
  const int grace;
  const int threshold;
  int debounce;
  bool listening;
  
public:
  TimedEvent callback;

  KnockSensor(const int pin, const int grace, const int threshold):pin(pin),grace(grace),threshold(threshold){
    debounce = 0;
    callback = NULL;
  }

  void enable(){
    pinMode(pin, INPUT);
    debounce = 0;
    listening = true;
  }

  void disable(){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    listening = false;
  }
  
  void sample(){
    if (!listening)
      return;
      
    debounce--;
    debounce = max(debounce, 0);
    
    if(debounce)
      return;
    
    int value;
    analogRead(pin);
    value = analogRead(pin);
    if( value > threshold){
      if(callback != NULL)
        callback(this);
      debounce = grace;
    }
  }
};

#endif
