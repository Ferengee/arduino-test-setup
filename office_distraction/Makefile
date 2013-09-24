libs = 
gccflags = -Wall -fpermissive -g -lm
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC ../../arduino-libs/OfficeDistraction/
includes =$(foreach d, $(include_paths), -I$d)

all: test_office_distraction


test_office_distraction : testOfficeDistraction.cpp PWMControl.o MessageQueueItem.o MessageQueueSorter.o TestSenderReceiver.o RFMessageControl.o Arduino.o DummySerial.o ButtonMessageControl.o
	$(cc) $(gccflags) testOfficeDistraction.cpp PWMControl.o MessageQueueItem.o MessageQueueSorter.o TestSenderReceiver.o RFMessageControl.o Arduino.o DummySerial.o ButtonMessageControl.o -o test_office_distraction $(includes) $(libs)

StateMachine.o: ../Arduino/libraries/MiniRPC/StateMachine.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/StateMachine.cpp -o StateMachine.o $(includes) $(libs)


MessageQueueItem.o: ../../arduino-libs/OfficeDistraction/MessageQueueItem.cpp
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/MessageQueueItem.cpp -o MessageQueueItem.o $(includes) $(libs)

RFMessageControl.o: ../../arduino-libs/OfficeDistraction/RFMessageControl.cpp 
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/RFMessageControl.cpp -o RFMessageControl.o $(includes) $(libs)


ButtonMessageControl.o: ../../arduino-libs/OfficeDistraction/ButtonMessageControl.cpp 
	$(cc) $(gccflags) -c  ../../arduino-libs/OfficeDistraction/ButtonMessageControl.cpp -o ButtonMessageControl.o $(includes) $(libs)


TestSenderReceiver.o: TestSenderReceiver.cpp 
	$(cc) $(gccflags) -c TestSenderReceiver.cpp -o TestSenderReceiver.o $(includes) $(libs)


PWMControl.o: ../../arduino-libs/OfficeDistraction/PWMControl.cpp 
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/PWMControl.cpp -o PWMControl.o $(includes) $(libs)


MessageQueueSorter.o: ../../arduino-libs/OfficeDistraction/MessageQueueSorter.cpp 
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/MessageQueueSorter.cpp -o MessageQueueSorter.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	rm test_office_distraction
