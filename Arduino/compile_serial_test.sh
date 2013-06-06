#!/bin/bash
#echo ${@}
#g++ -Wall testDummySerial.cpp -o test_dummy_serial -I ./ -I /usr/local/arduino-1.0.2/hardware/arduino/cores/arduino/Print.h -I ../arduino-libs/Observable -I ../arduino-libs/LCDForm -I ../arduino-libs/TempSensor -I ../arduino-libs/FourButtonInput -I ../arduino-libs/FourButtons
g++ -fpermissive testDummySerial.cpp DummySerial.cpp  -o test_dummy_serial -I ./  # -I /usr/local/arduino-1.0.2/hardware/tools/avr/lib/avr/include
