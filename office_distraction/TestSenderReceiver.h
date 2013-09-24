#ifndef TestSenderReceiver_h
#define TestSenderReceiver_h

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
  virtual bool receive(uint8_t * buf, uint8_t len);
  TestSenderReceiver * m_other;
private:
  MessageQueueItem m_buffer[MAXMESSAGECOUNT];
  int m_start;
  int m_end;
  int m_sendcount;
  
  
};

#endif