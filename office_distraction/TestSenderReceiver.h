#ifndef TestSenderReceiver_h
#define TestSenderReceiver_h


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "4340" // the port client will be connecting to 

#define MAXDATASIZE 80 // max number of bytes we can get at once 


#include <Arduino.h>
#include <MessageQueueItem.h>
#include <BaseSenderReceiver.h>

class TestSenderReceiver : public BaseSenderReceiver
{
public:
  TestSenderReceiver();
  virtual bool send(uint8_t * buf, uint8_t len);
  virtual bool have_message();
  virtual bool get_message(uint8_t * buf, uint8_t * len);
  virtual bool is_receiving();
  virtual bool is_transmitting();
  virtual void init(int txPin, int rxPin, int speed);
  
  int request_info(char type);
  int message_status();

  int startConnection();
private:
  int sockfd;
  int speed;

};

#endif