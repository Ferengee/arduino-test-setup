#include <stdio.h>
#include <string.h>
#include "aggregating_log.h"

AggregatingLog::AggregatingLog(){
  int i;

  for(i=0; i < AGGREGATING_LOG_SIZE; i++)
    values[i] = 0;

  cursor = 0;
  next = NULL;
}

void AggregatingLog::linkChain(AggregatingLog * buffers, int length){
  int i;  
  for(i = 0; i < length; i++){    
    if ( i > 0 )
      buffers[i - 1].next = &buffers[i];
  }
}

void AggregatingLog::add(int value){
  if (cursor == AGGREGATING_LOG_SIZE){
    cursor = 0;
    if (next != NULL){
      next->add(getAverage());
    } 
  }
  
  values[cursor] = value;
  cursor++;
}

int AggregatingLog::getAverage(){
  int acc = 0;
  int i;
  
  for (i = 0 ; i < AGGREGATING_LOG_SIZE; i++){
    acc = acc +   values[i];
  }
  return acc / AGGREGATING_LOG_SIZE;
}

int AggregatingLog::toJSON(char * out){
  int i;
  int written;
  char * cursor = out;
  
  for(i = 1; i < AGGREGATING_LOG_SIZE + 1; i++){
    int pos = (AGGREGATING_LOG_SIZE + this->cursor - i) % AGGREGATING_LOG_SIZE;
    
    written = snprintf(cursor, 6, ",%d", this->values[pos]);
    if (written > 0)
      cursor = cursor + written;
  }
    
  *out = '[';
  *cursor = ']'; 
  *(cursor + 1) = '\0';
  return (cursor - out) + 1;
}

int AggregatingLog::chainToJSON(char * out){
  char * cursor = out;
  int written;
  AggregatingLog * buffer = this;
  
  while(buffer != NULL){
    *cursor = ',';
    cursor++;
    written = buffer->toJSON(cursor);
    buffer = buffer->next;
    cursor = cursor + written;
  }
  *out = '[';
  *cursor = ']'; 
  *(cursor + 1) = '\0';
  return (cursor - out) + 1;
}


