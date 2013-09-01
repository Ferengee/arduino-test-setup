#include "Arduino.h"

void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
  
  
int analogRead(int pin) {
   char analog_read_buffer[5];

  int r;
  cout << "analogRead: " << pin << "\n";
  cin.getline(analog_read_buffer, 5);
  r = atoi(analog_read_buffer);
  return r;
}
void analogWrite(int pin, int value) {
   cout << "analogWrite: " << pin << ":" << value << "\n"; 
}

int digitalRead(int pin){
   char digital_read_buffer[5];

  int r;
  cout << "digitalRead: " << pin << "\n";
  cin.getline(digital_read_buffer, 5);
  r = atoi(digital_read_buffer);
  return r; 
}
void digitalWrite(int pin, int value){
     cout << "digitalWrite: " << pin << ":" << value << "\n"; 
}
void pinMode(int pin, int mode) {
       cout << "pinMode: " << pin << ":" << mode << "\n"; 
}

void delay(int timeout){
  usleep(timeout * 1000); 
}
 
unsigned long millis(void){
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}