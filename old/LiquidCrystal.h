#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <iostream>
#include <sstream>
using namespace std;

class Print {
  public:
  Print();
  virtual void print(basic_stringstream<char>::__string_type str){};
  virtual void print(char * str){};

};

class LiquidCrystal : public Print{
  public:
    LiquidCrystal(int i);
   void print(basic_stringstream<char>::__string_type str);
   void print(char * str);

   void setCursor(int x, int y);
  private:
    int x;
    int y;
};


#endif