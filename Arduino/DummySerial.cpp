#include "DummySerial.h"

using namespace std;

int getch(void )
{
  return getchar(); 
}
int ungetch(int c)
{
  return ungetc(c, stdin);
}
int endwin(void ){ return 0;}



Stream::Stream()
{
  m_timeout = DUMMY_SERIAL_TIMEOUT;
  m_available = true;
}

/*
size_t Stream::print(basic_stringstream<char>::__string_type  str)
{
  printf(str);
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
  m_timeout = new_timeout;
}

size_t Stream::print(const char str[])
{
  printf("%s", str);
  
  
  return 0;

}
size_t Stream::println(const char str[]){
  print(str);
  print("\n");
  return 0;
}


size_t Stream::print(uint8_t c)
{
  printf("%d", c);
  

  return 0;
}
size_t Stream::print(int c)
{
  printf("%d", c);
  

  return 0;
}
size_t Stream::print(float c)
{
  printf("%f", c);
  

  return 0;
}

size_t Stream::print(char c)
{
  printf("%c", c);
  

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
  while ((c = getchar()) > 0 && i < max){
    buffer[i] = (char)c;
    i++;
  }
  return i;
}

bool Stream::stdin_available()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = m_timeout / 1000;
    tv.tv_usec = 1000 * (m_timeout - (tv.tv_sec * 1000));

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); 
    return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv) > 0;
}

int Stream::read(void )
{
  if(stdin_available()){
    printf("hit\n");
    return getchar();
  }else{
    return -1;
  }
}

int Stream::peek(void )
{ 
  int r = read(); 
  if(r > -1)
    ungetc(r, stdin); 
  return r;
}


size_t Stream::readBytesUntil(char delim, char * buffer, int max){
  int i;
  int c;
  while ((c = getchar()) > 0 && i < max){
    if ((char)c == delim){
      ungetc(c, stdin);
      break;
    }
    buffer[i] = (char)c;
    i++;
  }
  return i;
}

DummySerial Serial;
