libs = 
gccflags = -Wall -fpermissive -g -lm
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/Schedulers ../Arduino/libraries/SimpleStateMachine ../Arduino/libraries/EventChannel 
includes =$(foreach d, $(include_paths), -I$d)

all: test_knock_statemachine


test_knock_statemachine : testStateMachine.cpp Schedulers.o DummySerial.o Arduino.o StateMachine.o EventChannel.o
	$(cc) $(gccflags) testStateMachine.cpp Schedulers.o DummySerial.o Arduino.o StateMachine.o EventChannel.o -o test_knock_statemachine $(includes) $(libs)

EventChannel.o: ../Arduino/libraries/EventChannel/EventChannel.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/EventChannel/EventChannel.cpp -o EventChannel.o $(includes) $(libs)

Schedulers.o: ../Arduino/libraries/Schedulers/Schedulers.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/Schedulers/Schedulers.cpp -o Schedulers.o $(includes) $(libs)


StateMachine.o: ../Arduino/libraries/SimpleStateMachine/SimpleStateMachine.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/SimpleStateMachine/SimpleStateMachine.cpp -o StateMachine.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	rm test_knock_statemachine
