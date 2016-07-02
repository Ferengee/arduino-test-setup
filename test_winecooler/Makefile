libs = 
gccflags = -Wall -fpermissive -g
cc = g++ --std=c++11

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

ZmqStream.o: ../Mocks/ZmqStream.cpp
	$(cc) $(gccflags) -c ../Mocks/ZmqStream.cpp -o ZmqStream.o $(includes) 


test: test_log test_handler test_server
	./test_log ; ./test_handler ; ./test_server

test_handler: tests/test_handler.cpp handler/handler.h handler/handler.cpp MockStream.o DummySerial.o
	$(cc) $(gccflags) tests/test_handler.cpp handler/handler.cpp MockStream.o DummySerial.o -o test_handler  $(includes) $(libs)

parse_http_request: tests/parse_http_request_from_stdin.cpp server/server.h server/server.cpp handler/handler.h handler/handler.cpp DummySerial.o
	$(cc) $(gccflags) tests/parse_http_request_from_stdin.cpp server/server.h server/server.cpp handler/handler.cpp DummySerial.o -o parse_http_request_from_stdin  $(includes) $(libs)

test_server: tests/test_server.cpp server/server.h server/server.cpp handler/handler.cpp MockStream.o DummySerial.o ZmqStream.o
	$(cc) $(gccflags) tests/test_server.cpp server/server.cpp handler/handler.cpp MockStream.o DummySerial.o ZmqStream.o -o test_server  $(includes) $(libs) -lzmq

server.o: server/server.h server/server.cpp
	$(cc) $(gccflags) -c server/server.cpp -o server.o $(includes) 

handler.o: handler/handler.h handler/handler.cpp
	$(cc) $(gccflags) -c handler/handler.cpp -o handler.o $(includes) 

MockEthernet.o: ../Mocks/Ethernet.cpp ../Mocks/Ethernet.h
	$(cc) $(gccflags) -c ../Mocks/Ethernet.cpp -o MockEthernet.o $(includes)

test_zmq_stream: tests/test_zmq_stream.cpp ZmqStream.o DummySerial.o
	$(cc) $(gccflags) tests/test_zmq_stream.cpp -o test_zmq_stream ZmqStream.o DummySerial.o $(includes) -lzmq

webserver_example: tests/webserver_example.cpp tests/webserver_example.ino DummySerial.o Arduino.o ZmqStream.o MockEthernet.o Schedulers.o server.o handler.o
	$(cc) $(gccflags) tests/webserver_example.cpp -o webserver_example DummySerial.o Arduino.o ZmqStream.o MockEthernet.o Schedulers.o server.o handler.o $(includes) -lzmq

test_log: tests/test_log.cpp lib.c lib.h
	$(cc) $(gccflags) tests/test_log.cpp lib.c -o test_log  $(includes) $(libs)

clean:
	find -name "*.o" -delete	
	find -name "*~" -delete 
	find ../../arduino-libs/OfficeDistraction/ -name "*.o"
	rm -f test_scheduler testrun test_server test_log test_handler test_zmq_stream webserver_example
