#include <DummySerial.h>

class MockStream : public Stream
{
public:
  virtual int read(void);
  void setSourceString(char * string, int len);

protected:  
  virtual int unread(char c);

private:
  int head;
  int max;
  char * sourceString;
};
