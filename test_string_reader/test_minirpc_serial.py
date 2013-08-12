#!/usr/bin/env python
import serial
import math
import time
import threading
import sys

com = serial.Serial('/dev/ttyACM0', 9600, timeout=2)
running = True
time.sleep(2)
def readSerial(com):
  while running:
    sys.stdout.write(com.read())


def test():
  time.sleep(1)

  for i in range(100):
    command = "led(11, %d)" % (math.sin(i) * 128 + 128)
    com.write(command)
    print(command)
    time.sleep(0.1)
  
t1 = threading.Thread(target=readSerial, args=(com,))

t1.start()
com.write("led(11,200)")
com.write("aap(11,200)")
com.write("(((")
test()
time.sleep(2)
running = False
t1.join()
