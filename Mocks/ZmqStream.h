#include <DummySerial.h>
#include "zhelpers.hpp"

class ZmqStream : public Stream
{
public:
  ZmqStream();
  virtual bool available(void);
  /* try to receive a message, if we have a current message return a int from that */
  virtual int read(void);
  void setSocket(zmq::socket_t *socket) {this->socket = socket;}

protected:  
  virtual int unread(char c);

private:
  zmq::message_t message;
  uint current_pos;
  zmq::socket_t *socket;
};
