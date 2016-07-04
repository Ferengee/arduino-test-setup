#include <stdio.h>
#include <stdint.h>
#include "dbg.h"
#include "aggregating_log/aggregating_log.h"


#define TRUE 1
#define FALSE 0

class AggregatingLogSpy : public AggregatingLog{
  /* expose internal state for testing */
public:
  int * getValues(){ return values;}
  AggregatingLog * getNext(){ return next;}  

};

int testCreateBuffer(){
  AggregatingLogSpy buffer;
  
  check(buffer.getValues()[0] == 0, "Expected values in buffer to be 0, but got: %d", buffer.getValues()[0]);
 
  return 0;
  error:
  return 1;
}

int testAddToBuffer(){
  
  uint8_t someValues[6] = {241, 34, 7, 8, 10, 20}; 
  
  AggregatingLogSpy buffer;

  buffer.add(someValues[0]);
  check(buffer.getValues()[0] == someValues[0], "Expected value 0 in buffer to be %d, but got: %d",someValues[0], buffer.getValues()[0]);

  buffer.add(someValues[1]);
  check(buffer.getValues()[1] == someValues[1], "Expected value 1 in buffer to be %d, but got: %d",someValues[1], buffer.getValues()[1]);
  
  buffer.add(someValues[2]);
  check(buffer.getValues()[2] == someValues[2], "Expected value 2 in buffer to be %d, but got: %d",someValues[2], buffer.getValues()[2]);
 
  buffer.add(someValues[3]);
  check(buffer.getValues()[0] == someValues[3], "Expected buffer to roundtrip buffer->values[0] == %d, but got: %d",someValues[3], buffer.getValues()[0]);
 
  return 0;
  error:
  return 1;
}

int testChainBuffer(){
  AggregatingLogSpy chain[2];
  
  AggregatingLog::linkChain(chain, 2);
  check(chain[0].getNext() == &chain[1], "expected chain to end at chain[1]");
  
  check(chain[1].getNext() == NULL, "expected chain to end at chain[1]");
  
  return 0;
  error:
  return 1;
}


int testGetAverage(){
  uint8_t someValues[6] = {241, 34, 7, 8, 10, 20}; 
  int i;
  
  AggregatingLog buffer;
  
  for(i = 0 ; i < AGGREGATING_LOG_SIZE; i++){
    buffer.add(someValues[i]);
  }
  
  uint8_t average = buffer.getAverage();
  uint8_t expected = (241 + 34 + 7) / 3;
  
  check(average == expected, "Expected buffer.getAverage() to be: %d, got: %d ", expected, average);
  
  return 0;
  error:
  return 1;
}

int testAddToBufferFlowsAverageIntoNext(){
  uint8_t someValues[6] = {241, 34, 7, 8, 10, 20}; 
  int i;
  
  AggregatingLogSpy chain[2];
  AggregatingLog::linkChain(chain, 2);
  
  for(i = 0 ; i < AGGREGATING_LOG_SIZE; i++){
    chain->add(someValues[i]);
  }
  
  uint8_t average = chain->getAverage();

  check(chain[1].getValues()[0] == 0, "Expected value 0 in buffer to be 0");
  
  chain->add(someValues[i++]); // added one more, buffer does roundtrip

  check(chain[1].getValues()[0] == average, "Expected value 0 in buffer to be: %d, but got: %d", average, chain[1].getValues()[0]);

  return 0;
  error:
  return 1;
}

int testToJSON(){
  
  uint8_t someValues[3] = {8, 10, 20}; 
  int i;
  
  char out[40];
  
  AggregatingLog buffer;
  
  buffer.toJSON(out);
  check(strncmp(out, "[0,0,0]", 7) == 0, "Expected analogBufferToJSON(&buffer) to format correctly");
  
  for(i = 0 ; i < AGGREGATING_LOG_SIZE; i++){
    buffer.add(someValues[i]);
  }
  buffer.toJSON(out);
  check(strncmp(out, "[1024,20,10]", 8) == 0, "Expected analogBufferToJSON(&buffer) to be [1024,20,10], got: %s", out);
  
  buffer.add(1023);
  buffer.toJSON(out);

  check(strncmp(out, "[20,10,8]", 8) == 0, "Expected analogBufferToJSON(&buffer) to be [20,10,8], got: %s", out);

  
  return 0;
  error:
  return 1;
}

int testChainToJSON(){
  uint8_t someValues[9] = {241, 34, 7, 8, 10, 20, 200,100,150}; 
  int i;
  
  char out[44];

  AggregatingLog chain[2];
  AggregatingLog::linkChain(chain, 2);
  
  for(i = 0 ; i < AGGREGATING_LOG_SIZE * 3; i++){
    chain->add(someValues[i]);
  }
  
  chain->toJSON(out);
  
  check(strncmp(out, "[150,100,200]", 17) == 0, "Expected chain->toJSON(out) to format correctly, got: %s", out);

  (chain + 1)->toJSON(out);
  check(strcmp(out, "[12,94,0]") == 0, "Expected chain->toJSON(out) to format correctly, got: %s", out);

  chain->add(1);
  
  (chain + 1)->toJSON(out);
  check(strcmp(out, "[150,12,94]") == 0, "Expected chain->toJSON(out) to format correctly, got: %s", out);

  chain->toJSON(out);

  check(strcmp(out, "[1,150,100]") == 0, "Expected chain->toJSON(out) to format correctly, got: %s", out);
  
  chain->chainToJSON(out);
  check(strncmp(out, "[[1,150,100],[150,12,94]]", 19) == 0, "Expected chainToJSON(chain) to be [[1,150,100],[150,12,94]], got: %s", out);
  
  return 0;
  error:
  return 1;  
}

int testChainToJSON_2(){
  uint8_t someValues[27] = {2,2,2,  8,8,8,    10,10,10,  4,4,4,  12,12,12, 20,20,20, 6,6,6,  1,1,1,    15,15,15 }; 
  int i;
  
  char out[100];

  AggregatingLog chain[4];
  AggregatingLog::linkChain(chain, 4);
  
  for(i = 0 ; i < AGGREGATING_LOG_SIZE * 9; i++){
    chain->add(someValues[i]);
  }
  
  chain->chainToJSON(out);
  check(strncmp(out, "[[15,15,15],[1,6,20],[12,6,0],[0,0,0]]", 19) == 0, "Expected chainToJSON(chain) to be  [[15,15,15],[1,6,20],[12,6,0],[0,0,0]] , got: %s", out);
  
  return 0;
  error:
  return 1;  
}


int main(void){
  int e = 0;
  printf("running log tests:");
  
  e = e & testCreateBuffer();
  e = e & testAddToBuffer();
  e = e & testChainBuffer();
  e = e & testGetAverage();
  e = e & testAddToBufferFlowsAverageIntoNext();
  e = e & testToJSON();
  e = e & testChainToJSON();
  e = e & testChainToJSON_2();

  
  if(e > 0)
    printf("   FAIL!\n");
  else
    printf("   OK!\n");

  return 0;
  

}