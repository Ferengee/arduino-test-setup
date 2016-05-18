libs = 
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/Schedulers
includes =$(foreach d, $(include_paths), -I$d)

all: test_scheduler


test_scheduler : test.cpp Schedulers.o Arduino.o DummySerial.o
	$(cc) $(gccflags) test.cpp Schedulers.o Arduino.o DummySerial.o -o test_scheduler $(includes) $(libs)


Schedulers.o: ../Arduino/libraries/Schedulers/Schedulers.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/Schedulers/Schedulers.cpp -o Schedulers.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	rm *~
	find ../../arduino-libs/OfficeDistraction/ -name "*.o" -delete
	rm test_scheduler