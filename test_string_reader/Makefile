libs = -lncurses
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC
includes =$(foreach d, $(include_paths), -I$d)

#all : sender receiver heartbeat_receiver heartbeat_broadcast main
all: test_state_machine test_minirpc


test_state_machine : test_state_machine.cpp BufferManager.o DummySerial.o StateMachine.o
	$(cc) $(gccflags) test_state_machine.cpp BufferManager.o ../Arduino/libraries/MiniRPC/StreamParser.cpp ../Arduino/libraries/MiniRPC/StateMachine.o ../Arduino/DummySerial.o -o test_state_machine $(includes) $(libs)
#g++ -Wall test_state_machine.cpp StateMachine.cpp BufferManager.cpp -I . -I ../Arduino ../Arduino/DummySerial.cpp ../Arduino/Arduino.cpp -l ncurses

test_minirpc: test_minirpc.cpp BufferManager.o DummySerial.o StateMachine.o MiniRPC.o Arduino.o
	$(cc) $(gccflags) test_minirpc.cpp ../Arduino/Arduino.o BufferManager.o ../Arduino/libraries/MiniRPC/StreamParser.cpp ../Arduino/libraries/MiniRPC/MiniRPC.o ../Arduino/libraries/MiniRPC/StateMachine.o ../Arduino/DummySerial.o -o test_minirpc $(includes) $(libs)


StateMachine.o: ../Arduino/libraries/MiniRPC/StateMachine.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/StateMachine.cpp -o ../Arduino/libraries/MiniRPC/StateMachine.o $(includes) $(libs)


BufferManager.o: ../Arduino/libraries/MiniRPC/BufferManager.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/BufferManager.cpp $(includes)

StreamParser.o: ../Arduino/libraries/MiniRPC/StreamParser.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/StreamParser.cpp $(includes)

MiniRPC.o : ../Arduino/libraries/MiniRPC/MiniRPC.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/MiniRPC.cpp -o ../Arduino/libraries/MiniRPC/MiniRPC.o $(includes)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o ../Arduino/DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o ../Arduino/Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	rm test_state_machine
