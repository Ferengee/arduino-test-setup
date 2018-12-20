
#ifndef MorseCode_h

#include <Arduino.h>

class MorseOutput
{
public:
  virtual MorseOutput * dot() = 0;
  virtual MorseOutput * dash() = 0;
  virtual MorseOutput * space() = 0;
};

class SerialMorseOutput : public MorseOutput
{
public:
  virtual MorseOutput * dot(){
    Serial.print(".");
    return this;
  }
  
  virtual MorseOutput * dash(){
    Serial.print("_");
    return this;
  }
  virtual MorseOutput * space(){
    Serial.print(" ");
    return this;
  }
};

class MorseCode
{
  MorseOutput * output;
public:
  MorseCode(MorseOutput * output):output(output){}
  
  MorseCode * send(char letter){
    switch(letter) {
      case 'a' :
        output->dot()->dash()->space();
        break;
      
      case 'b' :
        output->dash()->dot()->dot()->dot()->space();
        break;
      
      case 'c' :
        output->dash()->dot()->dash()->dot()->space();
        break;
      
      case 'd' :
        output->dash()->dot()->dot()->space();
        break;
      
      case 'e' :
        output->dot()->space();
        break;
      
      case 'i' :
        output->dot()->dot()->space();
        break;
      
      case 'l' :
        output->dot()->dash()->dot()->dot()->space();
        break;
      
      case 'm' :
        output->dash()->dash()->space();
        break;
      
      case 'o' :
        output->dash()->dash()->dash()->space();
        break;
      
      case 'p' :
        output->dot()->dash()->dash()->dot()->space();
        break;
      
      case 'r' :
        output->dot()->dash()->dot()->space();
        break;
      
      case 's' :
        output->dot()->dot()->dot()->space();
        break;
      
      case 't' :
        output->dash()->space();
        break;
      
      case 'u' :
        output->dot()->dot()->dash()->space();
        break;
      
      case 'w' :
        output->dot()->dash()->dash()->space();
        break;
    }
    return this;
  }
};

#endif
