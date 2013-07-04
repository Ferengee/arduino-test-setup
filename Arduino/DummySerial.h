#ifndef DummySerial_h
#define DummySerial_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <ncurses.h>

using namespace std;

class Stream 
{
public:  
  Stream();
  void begin(int rate=0) { rate; }
  bool available(void) {return m_available;}
  int read(void){ return getch();}
  int peek(void){ int r = getch(); ungetch(r); return r;}
  int timedRead(void) {return read();}
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
  size_t println(int c);
  size_t println(float c);
  size_t print(int c);
  size_t print(float c);

private:
  bool m_available;
  

};

class DummySerial : public Stream
{
  
};

extern DummySerial Serial;

#endif
