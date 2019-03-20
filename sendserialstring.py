import serial
import sys
import time


ser = serial.Serial(
    port='/dev/ttyS0',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=1)

print("connected to: " + ser.portstr)
input = 1
while True:
	input = raw_input(">> ")
	ser.write(input)
	time.sleep(1.0)
ser.close()
