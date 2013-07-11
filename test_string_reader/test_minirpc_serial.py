import serial
import math
import time
import thread

com = serial.Serial('/dev/ttyACM0')

def readSerial():
  while 1:
    print(com.readline())

thread.start_new_thread(readSerial, ())

for i in range(100):
  command = "led(11, %d)" % (math.sin(i) * 128 + 128)
  com.write(command)
  print(command)
  time.sleep(0.3)
  
    
