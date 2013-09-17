#include <Arduino.h>

#include <RFMessageControl.h>
#include <BaseSenderReceiver.h>
#include <MessageQueueSorter.h>
#include "../Arduino/DummySerial.h"
#include "../../arduino-libs/OfficeDistraction/MessageQueueSorter.h"

MessageQueueItem queue[MAXMESSAGECOUNT];
MessageQueueSorter sorter = MessageQueueSorter(queue);
MessageQueueItem * item;

char message[] = "Hallo";
void setup(){
    Serial.begin(9600);
  Serial.println(queue[0].getMessageId());
  Serial.println((uint8_t)sizeof(queue));
  int x;
  for (x=0; x < MAXMESSAGECOUNT; x++){
   queue[x].init((uint8_t) x, (uint8_t) x, (uint8_t *)message, 3);
   
  }
 
  
  MessageQueueItem ** sorted = sorter.getSortedQueue();
  for (x=0; x < MAXMESSAGECOUNT; x++){
   Serial.print(sorted[x]->getMessageId());
   Serial.print(":");
   Serial.println(sorted[x]->getRetriesLeft());
   
  }
  
  queue[4].decrementRetriesLeft();
  queue[6].decrementRetriesLeft();
  queue[7].decrementRetriesLeft();
   queue[4].decrementRetriesLeft();
  queue[6].decrementRetriesLeft();
  queue[7].decrementRetriesLeft();
   queue[4].decrementRetriesLeft();
  queue[6].decrementRetriesLeft();
  queue[7].decrementRetriesLeft();
  
  sorted = sorter.reorder();
  
    sorted = sorter.reorder();
  sorted = sorter.reorder();
  sorted = sorter.reorder();

  Serial.println("reordered...");
  
  for (x=0; x < MAXMESSAGECOUNT; x++){
   Serial.print(sorted[x]->getMessageId());
   Serial.print(":");
   Serial.print(sorted[x]->getRetriesLeft());
   Serial.print(":");
   Serial.println(sorted[x]->isDestroyed());
  }
    Serial.println("setup finished...");


}

void loop()
{
  static uint8_t mid=30;
  bool succes = sorter.getUnusedItem(&item);
  if(succes){
  item->init(2, mid++, (uint8_t *)message, 3);
    Serial.println(item->getMessageId());
  }else{
    Serial.println("out of unused items"); 
  }
  delay(1000);
}

int main()
{ 
  setup();
  for(;;){
    loop();
  }  
  endwin();
  return 0;
}
