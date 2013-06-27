libs = -lncurses
ggcflags = -g -Wall -fpermissive
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC
includes =$(foreach d, $(include_paths), -I$d)

#all : sender receiver heartbeat_receiver heartbeat_broadcast main
all: test_state_machine


test_state_machine : test_state_machine.cpp BufferManager.o DummySerial.o
	$(cc) $(gccflags) test_state_machine.cpp  StateMachine.cpp BufferManager.o  ../Arduino/DummySerial.o -o test_state_machine $(includes) $(libs)
#g++ -Wall test_state_machine.cpp StateMachine.cpp BufferManager.cpp -I . -I ../Arduino ../Arduino/DummySerial.cpp ../Arduino/Arduino.cpp -l ncurses

BufferManager.o: BufferManager.cpp
	$(cc) $(gccflags) -c BufferManager.cpp $(includes)


DummySerial.o: ../Arduino/DummySerial.cpp ../Arduino/Arduino.o
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp ../Arduino/Arduino.o -o ../Arduino/DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o ../Arduino/Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	rm test_state_machine
