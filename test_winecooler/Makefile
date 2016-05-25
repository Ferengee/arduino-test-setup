libs = 
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/Schedulers ../Mocks
includes =$(foreach d, $(include_paths), -I$d)

all: test_scheduler


test_scheduler : tests/test.cpp Schedulers.o Arduino.o DummySerial.o WineCooler.ino
	$(cc) $(gccflags) tests/test.cpp Schedulers.o Arduino.o DummySerial.o -o test_scheduler $(includes) $(libs)


Schedulers.o: ../Arduino/libraries/Schedulers/Schedulers.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/Schedulers/Schedulers.cpp -o Schedulers.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

MockStream.o: ../Mocks/MockStream.cpp
	$(cc) $(gccflags) -c ../Mocks/MockStream.cpp -o MockStream.o $(includes)

test: test_log test_handler test_server
	./test_log ; ./test_handler ; ./test_server

test_handler: tests/test_handler.cpp handler/handler.h handler/handler.cpp MockStream.o DummySerial.o
	$(cc) $(gccflags) tests/test_handler.cpp handler/handler.cpp MockStream.o DummySerial.o -o test_handler  $(includes) $(libs)

test_server: tests/test_server.cpp server/server.h server/server.cpp handler/handler.cpp MockStream.o DummySerial.o
	$(cc) $(gccflags) tests/test_server.cpp server/server.cpp handler/handler.cpp MockStream.o DummySerial.o -o test_server  $(includes) $(libs)


test_log: tests/test_log.cpp lib.c lib.h
	$(cc) $(gccflags) tests/test_log.cpp lib.c -o test_log  $(includes) $(libs)

clean:
	find -name "*.o" -delete	
	rm *~ 
	find ../../arduino-libs/OfficeDistraction/ -name "*.o"
	rm -f test_scheduler testrun test_server test_log test_handler