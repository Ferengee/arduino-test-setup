libs = 
gccflags = -Wall -fpermissive -g -lm
cc = g++

include_paths = ../Arduino/ . ../../arduino-libs/OfficeDistraction/ ../Arduino/libraries/EventChannel/ ../Arduino/libraries/Schedulers/ ../Arduino/libraries/SimpleStateMachine
includes =$(foreach d, $(include_paths), -I$d)
odp = ../../arduino-libs/OfficeDistraction/
ap = ../Arduino/
alp = ../Arduino/libraries/
objects = $(odp)PWMControl.o $(odp)MessageQueueItem.o $(odp)MessageQueueSorter.o TestSenderReceiver.o $(odp)RFMessageControl.o \
$(ap)Arduino.o $(ap)DummySerial.o $(odp)ButtonMessageControl.o

objects2 = TestSenderReceiver.o $(odp)PWMControl.o $(ap)Arduino.o $(ap)DummySerial.o $(alp)EventChannel/EventChannel.o $(alp)Schedulers/Schedulers.o $(alp)SimpleStateMachine/SimpleStateMachine.o
.cpp.o :
	$(cc) $(gccflags) $(includes) $(libs) -c $*.cpp -o $@


all: test_office_distraction test_transmit_statemachine test_reply_statemachine


test_office_distraction : testOfficeDistraction.cpp $(objects)
	$(cc) $(gccflags) testOfficeDistraction.cpp $(objects) -o test_office_distraction $(includes) $(libs)

test_transmit_statemachine : TestTransmitStatemachine.cpp $(objects2)
	$(cc) $(gccflags) TestTransmitStatemachine.cpp $(objects2) -o test_transmit_statemachine $(includes) $(libs)

test_reply_statemachine : TestReplyStatemachine.cpp $(objects2)
	$(cc) $(gccflags) TestReplyStatemachine.cpp $(objects2) -o test_reply_statemachine $(includes) $(libs)

clean:
	find -name "*.o" -delete
	rm test_office_distraction
	rm test_transmit_statemachine
	rm test_reply_statemachine
