#ifndef DummySerial_h
#define DummySerial_h
#define DUMMY_SERIAL_TIMEOUT 500
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

using namespace std;

int getch(void);

int ungetch(int c);

int endwin(void);


class Stream 
{
public:  
  Stream();
  void begin(int) { }
  bool available(void) {return m_available;}
  bool stdin_available(void);
  int read(void);
  int peek(void);
  void setTimeout(unsigned long timeout);
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
  size_t println(char c);

  size_t print(int c);
  size_t print(float c);
  size_t print(char c);


private:
  bool m_available;
  int m_timeout;
  

};

class DummySerial : public Stream
{
  
};

extern DummySerial Serial;

#endif
