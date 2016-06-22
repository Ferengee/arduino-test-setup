#include "ZmqStream.h"

ZmqStream::ZmqStream(){
  m_available = false;
  current_pos = 0;
}

int ZmqStream::read(void){
  if(message.size() > current_pos){
    return ((char*)message.data())[ current_pos++]; 
  } else {
     s_send (*socket, "World");
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

bool ZmqStream::available(void) {
  printf("%ld\n", message.size());
  if(m_available == false && socket->recv(&message), ZMQ_NOBLOCK){
    m_available = true;
    current_pos = 0;
  } else {
    m_available = false;
  }

  return m_available;
  
}