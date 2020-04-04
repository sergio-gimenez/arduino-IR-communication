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
unsigned long rbuf[4];

unsigned long msg;

boolean isEOT = false;

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


  if (count < 4) {

    msg |= rbuf[i / 8] << i;

    Serial.print("Shifted = ");
    Serial.println(msg, HEX);

    Serial.print("i/8 =");
    Serial.println(i / 8);

    Serial.print("i =");
    Serial.println(i);


    if (count == 3){
      Serial.println("\n\nEOT\n\n");
      isEOT = true;
    }
      
    count ++;
    i += 8;

  } if (isEOT) {
    Serial.println("Im in the else :)");
    msg = 0;
    count = 0;
    i = 0;
    isEOT = false;
  }

}
