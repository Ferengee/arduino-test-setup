libs = 
gccflags = -Wall -fpermissive -g -lm
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC ../Arduino/libraries/SimpleStateMachine ../Arduino/libraries/EventChannel 
includes =$(foreach d, $(include_paths), -I$d)

all: test_state_machine


test_state_machine : testStateMachine.cpp DummySerial.o Arduino.o StateMachine.o EventChannel.o
	$(cc) $(gccflags) testStateMachine.cpp DummySerial.o Arduino.o StateMachine.o EventChannel.o -o test_state_machine $(includes) $(libs)

EventChannel.o: ../Arduino/libraries/EventChannel/EventChannel.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/EventChannel/EventChannel.cpp -o EventChannel.o $(includes) $(libs)


StateMachine.o: ../Arduino/libraries/SimpleStateMachine/SimpleStateMachine.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/SimpleStateMachine/SimpleStateMachine.cpp -o StateMachine.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	rm test_state_machine