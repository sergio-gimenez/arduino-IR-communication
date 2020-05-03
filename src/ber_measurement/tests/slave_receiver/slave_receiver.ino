// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>

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
  Serial.begin(38400);           // start serial for output
  Serial.println("Ready to receive");
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  // Received int
  rbuf[i2c_bytes_count] = Wire.read();    
  Serial.print("Received byte ");
  Serial.println(i2c_bytes_count);
  Serial.println(rbuf[i2c_bytes_count]);         

  // Count for the first 5 bytes since packet length is 5 (msg + cheksum)
  if (i2c_bytes_count < 5) {
    if (!isChecksumComing) {
      msg |= rbuf[offset / 8] << offset;

      Serial.print("offset =");
      Serial.println(offset);

      Serial.print("offset/8 =");
      Serial.println(offset / 8);
    }

    if (i2c_bytes_count == 3) {
      isChecksumComing = true;
    }

    if (isChecksumComing && i2c_bytes_count == 4) {
      rx_checksum = rbuf[i2c_bytes_count];
      isEOT = true;
    }

    i2c_bytes_count ++;
    offset += 8;

  } if (isEOT) {
    Serial.print("Received message = ");
    Serial.println(msg, HEX);
    Serial.print("\nReceived checksum is: ");
    Serial.println(rx_checksum);

    for (int i = 0; i < 4; i++) {
      computed_checksum += rbuf[i];
    }
    computed_checksum = computed_checksum & 0xff;
    Serial.print("Computed checksum is: ");
    Serial.println(computed_checksum);

    if (computed_checksum == rx_checksum){
      Serial.println("Message is correct\n");
    }
    msg = 0;
    i2c_bytes_count = 0;
    offset = 0;
    isEOT = false;
    isChecksumComing = false;
    computed_checksum = 0;
  }
}
