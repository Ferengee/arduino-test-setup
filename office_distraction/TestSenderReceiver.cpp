#include "TestSenderReceiver.h"

TestSenderReceiver::TestSenderReceiver()
{

}

void TestSenderReceiver::init(int txPin, int rxPin, int speed)
{
  if(startConnection() > 0)
    exit(1);
  this->speed = speed;
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
  ::send(sockfd,  out, MAXDATASIZE, 0);
  return true;
}


bool TestSenderReceiver::have_message()
{
  return (message_status() == 1);
}


int TestSenderReceiver::message_status()
{
  int numbytes;
  char buf[MAXDATASIZE];
  int result = 0;
  ::send(sockfd, "a:", 2, 0);
  if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
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
