import serial
ser = serial.Serial ("/dev/ttyAMA0")    #Open named port 
ser.baudrate = 9600                     #Set baud rate to 9600
while True:
	print  ser.read(10)                     #Read ten characters from serial port to data
ser.close()


