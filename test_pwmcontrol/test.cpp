#include <PWMControl.h>

PWMControl control = PWMControl(4);

int main()
{
  control.on(200);
  control.update();
  control.off();
  control.update();
  control.sine(0,100,255, 255);
  unsigned long int now = millis();
  while ((millis() - now) < 200){
    control.update();
    delay (2);
  }
  
}
