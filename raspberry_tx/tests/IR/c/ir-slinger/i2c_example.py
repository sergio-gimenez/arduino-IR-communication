from smbus import SMBus
import time

addr = 0x8 # bus address
bus = SMBus(1) # indicates /dev/ic2-1
msg_bytes = [0,0,0,0,0] 

f = open("expected_message.txt", "rb")
expected_message = int(f.read())

for i in range(5):
    msg_bytes[i] = expected_message >> (8*i) & 0xff
    if i == 4: 
        msg_bytes[4] = sum(bytearray(msg_bytes))
    bus.write_byte(addr, msg_bytes[i])
    time.sleep(0.07)


#bus.write_i2c_block_data(addr, 0, msg_bytes)
print ("Message sent: "+ hex(expected_message))

