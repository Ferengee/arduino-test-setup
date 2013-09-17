libs = 
gccflags = -Wall -fpermissive -g -lm
cc = g++

include_paths = ../Arduino . ../Arduino/libraries/MiniRPC ../../arduino-libs/OfficeDistraction/
includes =$(foreach d, $(include_paths), -I$d)

all: test_office_distraction


test_office_distraction : testOfficeDistraction.cpp MessageQueueItem.o MessageQueueSorter.o RFMessageControl.o BaseSenderReceiver.o Arduino.o DummySerial.o
	$(cc) $(gccflags) testOfficeDistraction.cpp ../../arduino-libs/OfficeDistraction/MessageQueueItem.o ../../arduino-libs/OfficeDistraction/MessageQueueSorter.o ../../arduino-libs/OfficeDistraction/BaseSenderReceiver.o ../../arduino-libs/OfficeDistraction/RFMessageControl.o ../Arduino/Arduino.o ../Arduino/DummySerial.o -o test_office_distraction $(includes) $(libs)

StateMachine.o: ../Arduino/libraries/MiniRPC/StateMachine.cpp
	$(cc) $(gccflags) -c ../Arduino/libraries/MiniRPC/StateMachine.cpp -o ../Arduino/libraries/MiniRPC/StateMachine.o $(includes) $(libs)


MessageQueueItem.o: ../../arduino-libs/OfficeDistraction/MessageQueueItem.cpp
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/MessageQueueItem.cpp -o ../../arduino-libs/OfficeDistraction/MessageQueueItem.o $(includes) $(libs)

RFMessageControl.o: ../../arduino-libs/OfficeDistraction/RFMessageControl.cpp 
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/RFMessageControl.cpp -o ../../arduino-libs/OfficeDistraction/RFMessageControl.o $(includes) $(libs)

BaseSenderReceiver.o: ../../arduino-libs/OfficeDistraction/BaseSenderReceiver.cpp 
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/BaseSenderReceiver.cpp -o ../../arduino-libs/OfficeDistraction/BaseSenderReceiver.o $(includes) $(libs)

MessageQueueSorter.o: ../../arduino-libs/OfficeDistraction/MessageQueueSorter.cpp 
	$(cc) $(gccflags) -c ../../arduino-libs/OfficeDistraction/MessageQueueSorter.cpp -o ../../arduino-libs/OfficeDistraction/MessageQueueSorter.o $(includes) $(libs)

DummySerial.o: ../Arduino/DummySerial.cpp 
	$(cc) $(gccflags) -c ../Arduino/DummySerial.cpp -o ../Arduino/DummySerial.o $(includes) $(libs)

Arduino.o: ../Arduino/Arduino.cpp
	$(cc) $(gccflags) -c ../Arduino/Arduino.cpp -o ../Arduino/Arduino.o $(includes)

clean:
	find -name "*.o" -delete
	find ../../arduino-libs/OfficeDistraction/ -name "*.o" -delete
	rm test_office_distraction