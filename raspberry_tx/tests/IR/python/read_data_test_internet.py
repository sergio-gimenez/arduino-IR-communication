import serial
import time

ser = serial.Serial ("/dev/ttyAMA0")
ser.baudrate  = 2400

while 1:
	data = ser.read(1) # read 1 byte at a time
	print ord(data)	   # the data read in character, ord will convert to ASCII value
	time.sleep(0.5)
