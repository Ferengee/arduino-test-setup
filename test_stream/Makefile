libs = 
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/Schedulers
includes =$(foreach d, $(include_paths), -I$d)



DummySerial.o: ../Arduino/DummySerial.cpp ../Arduino/DummySerial.h
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

test: testrun
	./testrun

testrun: testrun.c DummySerial.o
	$(cc) $(gccflags) DummySerial.o testrun.c -o testrun  $(includes) $(libs)

clean:
	find -name "*.o" -delete	
	rm *~ 
	find ../../arduino-libs/OfficeDistraction/ -name "*.o"
	rm test_scheduler testrun; echo ok
