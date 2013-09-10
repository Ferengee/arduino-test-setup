#include <PWMControl.h>

PWMControl control = PWMControl(4);

int main()
{
  control.on(200);
  control.update();
  control.off();
  control.update();
  control.sine(0,10,255, 10);
  while (!control.isOff()){
    control.update();
    delay (2);
  }
  
}
