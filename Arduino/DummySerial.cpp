#include "DummySerial.h"

using namespace std;

DummySerial::DummySerial()
{
  initscr();
  timeout(5000000);
  m_available = false;
}

/*
size_t DummySerial::print(basic_stringstream<char>::__string_type  str)
{
  printw(str);
  return 0;
}

size_t DummySerial::println(basic_stringstream<char>::__string_type  str)
{
  print(str);
  print("\n");
  return 0;
}
*/

size_t DummySerial::print(const char str[])
{
  printw(str);
  refresh();
  return 0;

}
size_t DummySerial::println(const char str[]){
  print(str);
  print("\n");
  return 0;
}


size_t DummySerial::print(uint8_t c)
{
  printw("%d", c);
  return 0;
}
size_t DummySerial::println(uint8_t c){
  print(c);
  print("\n");
  return 0;
}

size_t DummySerial::readBytes(char * buffer, int max){
  int i;
  int c;
  while (c = getch() > 0 && i < max){
    buffer[i] = (char)c;
    i++;
  }
  return i;
}

size_t DummySerial::readBytesUntil(char delim, char * buffer, int max){
  int i;
  int c;
  while (c = getch() > 0 && i < max){
    if ((char)c == delim){
      ungetch(c);
      break;
    }
    buffer[i] = (char)c;
    i++;
  }
  return i;
}

DummySerial Serial;
