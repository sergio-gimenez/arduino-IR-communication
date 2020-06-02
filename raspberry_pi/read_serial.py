import serial
ser = serial.Serial('/dev/ttyACM0',38400)
while True:
	read_serial=ser.readline()
	print(read_serial)

        f = open("output.txt", "a")
        f.write(read_serial)
        f.close()
