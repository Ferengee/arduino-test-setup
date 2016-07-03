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
  virtual bool available(void) {return m_available;}
  bool stdin_available(void);
  virtual int read(void);
  int peek(void);
  void setTimeout(unsigned long timeout);
  int timedRead(void) {return read();}
  int parseInt(char skipChar);
  int parseInt(void);
  float parseFloat(){return 13.2;}
  size_t readBytes(char * buffer, int max);
  size_t readBytesUntil(char delim, char * buffer, int max);

 /* size_t print(basic_stringstream<char>::__string_type  str);
  size_t println(basic_stringstream<char>::__string_type  str);*/

  virtual size_t print(const char str[]);  
  virtual size_t println(const char str[]);
  virtual size_t print(const std::string str);
  virtual size_t println(const std::string str);
  virtual size_t println(){return 0;}


  virtual size_t print(uint8_t c);
  virtual size_t println(uint8_t c);
  virtual size_t println(int c);
  virtual size_t println(float c);
  virtual size_t println(char c);

  virtual size_t print(int c);
  virtual size_t print(float c);
  virtual size_t print(char c);
protected:
  virtual int unread(char c);
  bool m_available;

private:
  int getSign(char c);
  int isNumeric(char c);
  
  int m_timeout;
  

};

class DummySerial : public Stream
{
public:
  virtual operator bool(){ return true;}

};

extern DummySerial Serial;

#endif
