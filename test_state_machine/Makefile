libs = 
gccflags = -Wall -fpermissive -g -lm
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC ../Arduino/libraries/SimpleStateMachine
includes =$(foreach d, $(include_paths), -I$d)

all: test_state_machine


test_state_machine : testStateMachine.cpp DummySerial.o Arduino.o StateMachine.o
	$(cc) $(gccflags) testStateMachine.cpp DummySerial.o Arduino.o StateMachine.o -o test_state_machine $(includes) $(libs)

StateMachine.o: ../Arduino/libraries/SimpleStateMachine/SimpleStateMachine.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/SimpleStateMachine/SimpleStateMachine.cpp -o StateMachine.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	find ../../arduino-libs/StateMachine/ -name "*.o" -delete
	rm test_state_machine