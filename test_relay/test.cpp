#include <Relay.h>

Relay relay = Relay(4);

int main()
{
  relay.on();
  relay.off();
  relay.toggle();
}
