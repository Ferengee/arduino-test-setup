#include <PWMControl.h>

PWMControl control = PWMControl(4);

int main()
{
  control.on(200);
  control.update();
  control.off();
  control.update();
  control.sine(0,100,255, 255);
  
  int i ;
  for (i=0; i < 90; i++){
    control.update();
    delay (100);
  }
  
}
