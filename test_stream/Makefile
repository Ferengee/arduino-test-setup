libs = 
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/Schedulers ../Mocks
includes =$(foreach d, $(include_paths), -I$d)


DummySerial.o: ../Arduino/DummySerial.cpp ../Arduino/DummySerial.h
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

MockStream.o: ../Mocks/MockStream.cpp
	$(cc) $(gccflags) -c ../Mocks/MockStream.cpp -o MockStream.o $(includes)

test: testrun
	./testrun

testrun: testrun.c DummySerial.o MockStream.o
	$(cc) $(gccflags) DummySerial.o MockStream.o testrun.c -o testrun  $(includes) $(libs)

clean:
	find -name "*.o" -delete	
	rm *~ 
	find ../../arduino-libs/OfficeDistraction/ -name "*.o"
	rm test_scheduler testrun; echo ok
