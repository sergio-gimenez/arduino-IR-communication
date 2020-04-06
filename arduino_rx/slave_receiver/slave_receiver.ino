// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int count;

//unsigned long rbuf[4] = {170, 170, 170, 170};
unsigned long rbuf[5];

int rx_checksum, computed_checksum;

unsigned long msg;

boolean isEOT = false;

boolean isChecksumComing = false;

int i;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  Serial.println("Ready to receive");

  //msg = rbuf[0 / 8] << 0 | rbuf[8 / 8] << 8 | rbuf[16 / 8] << 16 | rbuf[24 / 8] << 24;

  //Serial.print("Shifted test message = ");
  //Serial.println(msg);

}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  rbuf[count] = Wire.read();    // receive byte as an integer
  Serial.print("Received byte ");
  Serial.println(count);
  Serial.println(rbuf[count]);         // print the integer

  if (count < 5) {
    if (!isChecksumComing) {
      msg |= rbuf[i / 8] << i;

      Serial.print("i =");
      Serial.println(i);

      Serial.print("i/8 =");
      Serial.println(i / 8);
    }

    if (count == 3) {
      isChecksumComing = true;
    }

    if (isChecksumComing && count == 4) {
      rx_checksum = rbuf[count];
      isEOT = true;
    }

    count ++;
    i += 8;

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
    msg = 0;
    count = 0;
    i = 0;
    isEOT = false;
    isChecksumComing = false;
    computed_checksum = 0;
  }



}
