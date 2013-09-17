#!/bin/bash
#echo ${@}
#g++ -Wall testDummySerial.cpp -o test_dummy_serial -I ./ -I /usr/local/arduino-1.0.2/hardware/arduino/cores/arduino/Print.h -I ../arduino-libs/Observable -I ../arduino-libs/LCDForm -I ../arduino-libs/TempSensor -I ../arduino-libs/FourButtonInput -I ../arduino-libs/FourButtons
g++ testSortedQueue.cpp ../Arduino/*.cpp $(cat office_distraction_files.txt) -o test_sorted_queue -lncurses -I ./ -I ../../arduino-libs/OfficeDistraction  -I ../Arduino # -I /usr/local/arduino-1.0.2/hardware/tools/avr/lib/avr/include
