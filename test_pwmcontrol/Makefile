libs = 
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC ../../arduino-libs/OfficeDistraction/
includes =$(foreach d, $(include_paths), -I$d)

all: test_relay


test_relay : test.cpp PWMControl.o Arduino.o DummySerial.o
	$(cc) $(gccflags) test.cpp ../../arduino-libs/OfficeDistraction/PWMControl.o ../Arduino/Arduino.o ../Arduino/DummySerial.o -o test_pwmcontrol $(includes) $(libs)


PWMControl.o: ../../arduino-libs/OfficeDistraction/PWMControl.cpp
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/PWMControl.cpp -o ../../arduino-libs/OfficeDistraction/PWMControl.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o ../Arduino/DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o ../Arduino/Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	find ../../arduino-libs/OfficeDistraction/ -name "*.o" -delete
	rm test_pwmcontrol