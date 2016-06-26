#ifndef _ZMQSTREAM_
#define _ZMQSTREAM_

#include <DummySerial.h>
#include <zmq.hpp>

class ZmqStream : public Stream
{
public:
  ZmqStream();
  ZmqStream(const ZmqStream &other);

  virtual bool available(void);
  /* try to receive a message, if we have a current message return a int from that */
  virtual int read(void);
  void setSocket(zmq::socket_t *socket) {this->socket = socket;}
  virtual int write(std::string str);
protected:  
  virtual int unread(char c);

private:
  zmq::message_t *message;
  unsigned long int current_pos;
  zmq::socket_t *socket;
};
#endif
