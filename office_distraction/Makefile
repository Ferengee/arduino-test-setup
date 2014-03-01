libs = 
gccflags = -Wall -fpermissive -g -lm
cc = g++

include_paths = ../Arduino/ . ../../arduino-libs/OfficeDistraction/
includes =$(foreach d, $(include_paths), -I$d)
odp = ../../arduino-libs/OfficeDistraction/
ap = ../Arduino/
objects = $(odp)PWMControl.o $(odp)MessageQueueItem.o $(odp)MessageQueueSorter.o TestSenderReceiver.o $(odp)RFMessageControl.o $(ap)Arduino.o $(ap)DummySerial.o $(odp)ButtonMessageControl.o

.cpp.o :
	$(cc) $(gccflags) $(includes) $(libs) -c $*.cpp -o $@


all: test_office_distraction


test_office_distraction : testOfficeDistraction.cpp $(objects)
	$(cc) $(gccflags) testOfficeDistraction.cpp $(objects) -o test_office_distraction $(includes) $(libs)

clean:
	find -name "*.o" -delete
	rm test_office_distraction
