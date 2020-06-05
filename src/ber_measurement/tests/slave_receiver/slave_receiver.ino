// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>

#define LAST_IR_MESSAGE_TIMEOUT 5000
#define I2C_PKT_LENGTH 4
#define I2C_BUS_ADDRESS 8

int i2c_bytes_count;

//unsigned long rbuf[4] = {170, 170, 170, 170};
unsigned long rbuf[5];

int rx_checksum, computed_checksum;

unsigned long msg;

boolean isEOT = false;

boolean isChecksumComing = false;

int offset;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(2400);           // start serial for output
  Serial.println("Ready to receive");
}

void loop() { }

void receiveEvent(int howMany) {

  // Received int
  rbuf[i2c_bytes_count] = Wire.read();    
  Serial.print("Received byte ");
  Serial.println(i2c_bytes_count);
  Serial.println(rbuf[i2c_bytes_count]);         

  if (i2c_bytes_count < I2C_PKT_LENGTH) {
      msg |= rbuf[offset / 8] << offset;

      Serial.print("offset =");
      Serial.println(offset);

      Serial.print("offset/8 =");
      Serial.println(offset / 8);

    if (i2c_bytes_count == (I2C_PKT_LENGTH - 1)) {
      isEOT = true;
    }

    i2c_bytes_count ++;
    offset += 8;

  } if (isEOT) {
    Serial.print("Received message = ");
    Serial.println(msg, HEX);

    msg = 0;
    i2c_bytes_count = 0;
    offset = 0;
    isEOT = false;
    isChecksumComing = false;
    computed_checksum = 0;
  }
}
