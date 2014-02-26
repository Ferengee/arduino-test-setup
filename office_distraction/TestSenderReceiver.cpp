#include "TestSenderReceiver.h"

TestSenderReceiver::TestSenderReceiver()
{

}

void TestSenderReceiver::init(int txPin, int rxPin, int speed)
{
  if(startConnection() > 0)
    exit(1);
  this->speed = speed;
  char buf[MAXDATASIZE] = {0};
  char out[MAXDATASIZE + 2] = {0};
  out[0]='b';
  out[1]=':';
  itoa(speed, buf);
  memcpy(&out[2],buf, MAXDATASIZE);
  printf("bautrate: %s\n", out);
  ::send(sockfd,  out, MAXDATASIZE + 2, 0);
}


int TestSenderReceiver::startConnection(){
  struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((this->sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(this->sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to connect to RF emulation server at 127.0.0.1:4340\n");
        return 2;
    }
    freeaddrinfo(servinfo); // all done with this structure
    return 0;
}

bool TestSenderReceiver::send(uint8_t* buf, uint8_t len)
{
  char out[MAXDATASIZE + 2] = {0};
  out[0]='s';
  out[1]=':';
  memcpy(&out[2],buf, len);
  printf("%s", out);
  ::send(sockfd,  out, MAXDATASIZE + 2, 0);
  return true;
}

bool TestSenderReceiver::is_receiving()
{
      return request_info('r') == 1;
}

bool TestSenderReceiver::is_transmitting()
{
      return request_info('t') == 1;
}

bool TestSenderReceiver::have_message()
{
  return (message_status() == 1);
}


int TestSenderReceiver::message_status()
{
  return request_info('a');
}

int TestSenderReceiver::request_info(char type)
{
  int numbytes;
  char buf[MAXDATASIZE];
  int result = 0;
  
  char out[2];
  out[0] = type;
  out[1] = ':';
  ::send(sockfd, out, 2, 0);
  if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv");
        exit(1);
  }
  buf[1] = '\0';
  if(numbytes > 0)
    result = atoi((char *)buf);
  
  return result;
}

bool TestSenderReceiver::get_message(uint8_t * buf, uint8_t * len)
{ 
  int status = message_status();
  if (status > 0){
    ::send(sockfd, "g:", 2, 0);
    if ((*len = recv(sockfd, buf, (size_t)*len, 0)) == -1) {
          perror("recv");
          exit(1);
    }
  }
  return status == 1;
}
