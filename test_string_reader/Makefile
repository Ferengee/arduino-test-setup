libs = -lncurses
gccflags = -Wall -fpermissive -g
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC
includes =$(foreach d, $(include_paths), -I$d)

#all : sender receiver heartbeat_receiver heartbeat_broadcast main
all: test_state_machine


test_state_machine : test_state_machine.cpp BufferManager.o DummySerial.o StateMachine.o
	$(cc) $(gccflags) test_state_machine.cpp BufferManager.o ../Arduino/libraries/MiniRPC/StreamParser.cpp ../Arduino/libraries/MiniRPC/StateMachine.o ../Arduino/DummySerial.o -o test_state_machine $(includes) $(libs)
#g++ -Wall test_state_machine.cpp StateMachine.cpp BufferManager.cpp -I . -I ../Arduino ../Arduino/DummySerial.cpp ../Arduino/Arduino.cpp -l ncurses

StateMachine.o: ../Arduino/libraries/MiniRPC/StateMachine.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/StateMachine.cpp -o ../Arduino/libraries/MiniRPC/StateMachine.o $(includes) $(libs)


BufferManager.o: BufferManager.cpp
	$(cc) $(gccflags) -c BufferManager.cpp $(includes)

StreamParser.o: ../Arduino/libraries/MiniRPC/StreamParser.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/StreamParser.cpp $(includes)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o ../Arduino/DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o ../Arduino/Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	rm test_state_machine