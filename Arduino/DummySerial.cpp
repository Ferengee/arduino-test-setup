#include "DummySerial.h"

using namespace std;

Stream::Stream()
{
  initscr();
  timeout(DUMMY_SERIAL_TIMEOUT);
  m_available = true;
}

/*
size_t Stream::print(basic_stringstream<char>::__string_type  str)
{
  printw(str);
  return 0;
}

size_t Stream::println(basic_stringstream<char>::__string_type  str)
{
  print(str);
  print("\n");
  return 0;
}
*/
void Stream::setTimeout(long unsigned int new_timeout)
{
  timeout(new_timeout);
}

size_t Stream::print(const char str[])
{
  printw(str);
  refresh();
  return 0;

}
size_t Stream::println(const char str[]){
  print(str);
  print("\n");
  return 0;
}


size_t Stream::print(uint8_t c)
{
  printw("%d", c);
  refresh();

  return 0;
}
size_t Stream::print(int c)
{
  printw("%d", c);
  refresh();

  return 0;
}
size_t Stream::print(float c)
{
  printw("%f", c);
  refresh();

  return 0;
}

size_t Stream::print(char c)
{
  printw("%c", c);
  refresh();

  return 0;
}
size_t Stream::println(uint8_t c){
  print(c);
  print("\n");
  return 0;
}
size_t Stream::println(int c){
  print(c);
  print("\n");
  return 0;
}

size_t Stream::println(float c){
  print(c);
  print("\n");
  return 0;
}

size_t Stream::println(char c){
  print(c);
  print("\n");
  return 0;
}

size_t Stream::readBytes(char * buffer, int max){
  int i;
  int c;
  while ((c = getch()) > 0 && i < max){
    buffer[i] = (char)c;
    i++;
  }
  return i;
}

size_t Stream::readBytesUntil(char delim, char * buffer, int max){
  int i;
  int c;
  while ((c = getch()) > 0 && i < max){
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
