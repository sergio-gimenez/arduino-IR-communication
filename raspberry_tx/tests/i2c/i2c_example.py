from smbus import SMBus
import time

addr = 0x8 # bus address
bus = SMBus(1) # indicates /dev/ic2-1

f = open("expected_message.txt", "r")
expected_message = f.read()

byte1 = int(expected_message[24:], 2)
byte2 = int(expected_message[16:-9], 2)
byte3 = int(expected_message[8:-17], 2)
byte4 = int(expected_message[:-25], 2)

bus.write_byte(addr, byte1)
#time.sleep(0.5)
bus.write_byte(addr, byte2)
#time.sleep(0.5)
#bus.write_byte(addr, byte3)
time.sleep(0.5)
#bus.write_byte(addr, byte4)

print ("Message sent: "+ expected_message)

