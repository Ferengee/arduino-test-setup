#ifndef DummySerial_h
#define DummySerial_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <ncurses.h>

using namespace std;

class DummySerial 
{
public:  
  DummySerial();
  void begin(int rate=0) { 0; }
  bool available(void) {return false;}
  int read(void){ return 0;}
  int parseInt(){return 7;}
  float parseFloat(){return 13.2;}
  size_t readBytes(char * buffer, int max);
  size_t readBytesUntil(char delim, char * buffer, int max);

 /* size_t print(basic_stringstream<char>::__string_type  str);
  size_t println(basic_stringstream<char>::__string_type  str);*/

  size_t print(const char str[]);
  size_t println(const char str[]);
  size_t print(uint8_t c);
  size_t println(uint8_t c);

};
extern DummySerial Serial;

#endif
