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

//unsigned long rbuf[4] = {170, 170, 170, 170};
unsigned long rbuf[4];

int offset;


// I2C variables \\

int i2c_bytes_count;
unsigned long i2c_rbuf[I2C_PKT_LENGTH];
unsigned long msg;
boolean isEOT = false;
unsigned long rcv_i2c_pkts;
boolean isI2CinBuf = false;
unsigned long expected_message = 0b00000000000000000000000000000000;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(handle_i2c_event); // register event
  Serial.begin(2400);           // start serial for output
  Serial.println("Ready to receive");
}

void loop() { }
// Function that executes whenever data is received from master
// This function is registered as an event, see setup()
void handle_i2c_event() {

  rcv_i2c_pkts++;

  i2c_rbuf[i2c_bytes_count] = Wire.read();

  // TODO I could wrap the bit shifting in a function to make it more readable
  if (i2c_bytes_count < I2C_PKT_LENGTH) {

    msg |= i2c_rbuf[offset / 8] << offset;

    if (i2c_bytes_count == (I2C_PKT_LENGTH - 1)) {
      isEOT = true;
    }

    i2c_bytes_count ++;
    offset += 8;
  }

  if (isEOT) {

    // Verify that received message is not a i2c scan
    if (!(msg == 0xFFFFFFFF)) {

      Serial.print("I2C Received message = ");
      Serial.println(msg, HEX);

      expected_message = msg;
    }

    // End of i2c transmission. Reinitialize variables
    msg = 0;
    i2c_bytes_count = 0;
    offset = 0;
    isEOT = false;
    isI2CinBuf = true;
  }
}
