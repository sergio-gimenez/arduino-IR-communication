#!/usr/bin/env python2.7

from smbus import SMBus
import subprocess

I2C_PKT_LENGTH = 4  # In bytes
I2C_BUS_ADDR = 0x8
I2C_BUS = SMBus(1)  # indicates /dev/ic2-1

FILE_WHERE_MSG_IS_STORED = "expected_message.txt"


def main():
    msg_to_send = [0, 0, 0, 0]
    msg_read_from_file = read_32bit_msg_from_file(FILE_WHERE_MSG_IS_STORED)
    send_32bit_msg_i2c(msg_read_from_file, msg_to_send,
                       I2C_PKT_LENGTH, I2C_BUS_ADDR, I2C_BUS)
    print ("i2c Message sent: " + hex(msg_read_from_file), "("+bin(msg_read_from_file)+")")


def read_32bit_msg_from_file(filename):
    f = open("expected_message.txt", "rb")
    return int(f.read(32), base=2)


def send_32bit_msg_i2c(msg_read_from_file, msg_to_send, msg_length, i2c_bus_addr, i2c_bus):
    for one_byte_from_msg in range(I2C_PKT_LENGTH):
        # Write each byte of the message in one position of msg_to_send[]
        msg_to_send[one_byte_from_msg] = msg_read_from_file >> (
            8*one_byte_from_msg) & 0xff

        try:
            i2c_bus.write_byte(i2c_bus_addr, msg_to_send[one_byte_from_msg])
        except IOError:
            # Send an i2c scan command from raspberry in order to try to fix the error in the communication.
            # TODO VERIFY --> It's supposed to work, but I would say it does not. But I think it coses the communication in the BER experiment
            subprocess.call(['i2cdetect', '-y', '1'])
            print("IOError detected. Some error appeared in the i2c bus")


if __name__ == "__main__":
    main()
