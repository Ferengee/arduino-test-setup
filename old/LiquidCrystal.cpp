#include <LiquidCrystal.h>
#include "WProgram.h"


Print::Print(){}

LiquidCrystal::LiquidCrystal(int i){
  x=i;
  y=i;
}

void LiquidCrystal::print(basic_stringstream<char>::__string_type  str){
  cout << "(" << x << ", " << y << ")" << str << "\n";
}

void LiquidCrystal::print(char *  str){
  cout << "(" << x << ", " << y << ")" << str << "\n";
}

void LiquidCrystal::setCursor(int x, int y)
{
    this->x = x;
    this->y = y;
}

