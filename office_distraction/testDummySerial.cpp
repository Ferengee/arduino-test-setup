#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Arduino.h"

#include "DummySerial.h"

using namespace std;

int main(){
  cout << "start\n";
  DummySerial s1 = DummySerial();
  s1.println("Test line ...");
  return 0;
}