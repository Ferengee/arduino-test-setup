#!/bin/bash
#echo ${@}
#g++ -Wall testDummySerial.cpp -o test_dummy_serial -I ./ -I /usr/local/arduino-1.0.2/hardware/arduino/cores/arduino/Print.h -I ../arduino-libs/Observable -I ../arduino-libs/LCDForm -I ../arduino-libs/TempSensor -I ../arduino-libs/FourButtonInput -I ../arduino-libs/FourButtons
g++ -fpermissive testSerialRPC.cpp ../Arduino/*.cpp $(cat serial_rpc_files.txt) -o test_serialrpc -lncurses -I ./ -I ../../arduino-libs/SerialRPC  -I ../Arduino # -I /usr/local/arduino-1.0.2/hardware/tools/avr/lib/avr/include
