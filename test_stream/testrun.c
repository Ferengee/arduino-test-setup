#include <stdio.h>
#include <stdint.h>
#include <DummySerial.h>
#include "dbg.h"
#include <MockStream.h>


#define TRUE 1
#define FALSE 0

int testCreateSerial(){
  
  Stream stream;
  
  check(TRUE, "Expect true");
 
  return 0;
error:
  return 1;
}

int testMockSerialRead(){
  
  MockStream stream;
  char source[] = "543";
  stream.setSourceString(source, 3);
  int i;
  int val=2;
  
  for (i=0; i<3; i++){
    check(stream.read() == source[i], "Expect stream.read to return %c", source[i]); 
  }
  
  val = stream.read();
  check(val == -1, "Expect to return -1 if the stream has ended, got %d", val);

  val = stream.read();
  val = stream.read();

  check(val == -1, "Expect to return -1 if the stream has ended, got %d", val);
  return 0;
error:
  return 1;
}

int testSerialParseInt()
{
  
  MockStream stream;
  char six[] = "6";
  char eight[] = "8";
  char twelve[] = "12";
  char minus_two[] = "-2";
  char minus_twothousandandfour[] = "-2004";
  char invalid[] = "invalid";
  
  struct option {
    char * input;
    int expected;
    int len;
  };
  
  struct option options[] = {
    {six, 6, 1},
    {eight, 8, 1},
    {twelve, 12, 2},
    {minus_two, -2, 2},
    {minus_twothousandandfour, -2004, 5},
    {invalid, 0, 7},
  };

  int val = 0;
  //int val = stream.parseInt();
  //check(val == 6, "expected 6 but got: %d", val);
  unsigned int i;
  for(i = 0; i < sizeof(options) / sizeof(struct option); i++){
    stream.setSourceString(options[i].input, options[i].len);
    val = stream.parseInt();
    check(val == options[i].expected, "expected %d but got: %d",options[i].expected, val);
  }  

  return 0;
error:
  
  return 1;
}


int testSerialParseIntSkipChar()
{
  
  MockStream stream;
  char dot_twothousandandfour[] = "2.004";
  char broken[] = {'2','3', '\0', '7', '3', '1', '\0'};

  char large_number[] = "10,324,345";
  char invalid[] = "invalid";
  
  
  struct option {
    char * input;
    int expected;
    int len;
    char skip;
  };
  
  struct option options[] = {
    {invalid, 0, 7, '.'},
    {dot_twothousandandfour, 2004, 5, '.'},
    {broken, 23, 7, '\0'},
    {large_number, 10324345, 10, ','},
  };
  
  int val = 0;

  unsigned int i;
  for(i = 0; i < sizeof(options) / sizeof(struct option); i++){
    stream.setSourceString(options[i].input, options[i].len);
    val = stream.parseInt(options[i].skip);
    check(val == options[i].expected, "expected %d but got: %d",options[i].expected, val);
  }  

  return 0;
error:
  
  return 1;
}

int main(void){
  int e = 0;
  printf("running tests:");
  
  e = e || testCreateSerial();
  e = e || testMockSerialRead();
  e = e || testSerialParseInt();
  e = e || testSerialParseIntSkipChar();
  
  if(e > 0)
    printf("   FAIL!\n");
  else
    printf("   OK!\n");

  return 0;
}
