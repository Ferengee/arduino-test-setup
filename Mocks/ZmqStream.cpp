#include "ZmqStream.h"

ZmqStream::ZmqStream() : Stream(){
  socket = NULL;
  m_available = false;
  current_pos = 0;
  message = new zmq::message_t;

}

ZmqStream::ZmqStream(const ZmqStream &other) : Stream(){
  socket = other.socket;
  m_available = other.m_available;
  current_pos = other.current_pos;
  message = other.message;
}


int ZmqStream::read(void){
  if(message->size() > current_pos){
    return  ((char*)message->data())[ current_pos++];
  } else {

     m_available = false;
     return -1;
  }
}

int ZmqStream::unread(char c){
    current_pos--;
    if (current_pos < 0){
      current_pos = 0;
      return -1;
    }
    return c;
}

int ZmqStream::write(std::string string){
  if( socket == NULL){
    return 0;
  }   
  zmq::message_t reply(string.size());
  memcpy(reply.data(), string.data(), string.size());

  return socket->send(reply);
}

bool ZmqStream::available(void) {
  if( socket == NULL || message == NULL){
    return false;
  }  
  
  if (m_available == true)
    return true;
  

  if(m_available == false && socket->recv(message, ZMQ_NOBLOCK)){
    m_available = true;
    current_pos = 0;
  } else {
    m_available = false;
  }

  return m_available;
  
}