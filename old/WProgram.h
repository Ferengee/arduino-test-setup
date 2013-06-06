#ifndef _WPROGRAM_H
#define _WPROGRAM_H

#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "LiquidCrystal.h"
using namespace std;

enum { LOW, HIGH };
enum { D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, A0, A1, A2, A3, A4, A5 };

typedef int boolean;
void reverse(char s[]);
void itoa(int n, char s[]);

int analogRead(int pin);
void analogWrite(int pin, int value);

int digitalRead(int pin);
void digitalWrite(int pin, int value);

void pinMode(int pin, int mode);

#endif