libs = -lncurses
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC ../../arduino-libs/OfficeDistraction/
includes =$(foreach d, $(include_paths), -I$d)

all: test_relay


test_relay : test.cpp Relay.o Arduino.o DummySerial.o
	$(cc) $(gccflags) test.cpp ../../arduino-libs/OfficeDistraction/Relay.o ../Arduino/Arduino.o ../Arduino/DummySerial.o -o test_relay $(includes) $(libs)


Relay.o: ../../arduino-libs/OfficeDistraction/Relay.cpp
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/Relay.cpp -o ../../arduino-libs/OfficeDistraction/Relay.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o ../Arduino/DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o ../Arduino/Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	find ../../arduino-libs/OfficeDistraction/ -name "*.o" -delete
	rm test_relay