
#include <Wire.h>

#define I2C_ADDR 8
#define MAX_32_BIT_VALUE 2147483647
#define I2C_PKT_LENGTH 4 //In bytes

long randNumber;
int splitted_msg[4];
int i2c_bytes_count;

void setup() {
  Serial.begin(2400);

  Serial.println("I2C Master is ready");
  Wire.begin();

  /*
     If analog input pin 0 is unconnected, random analog
     noise will cause the call to randomSeed() to generate
     different seed numbers each time the sketch runs.
     randomSeed() will then shuffle the random function.
  */
  randomSeed(analogRead(0));
  delay(10000);
}
void loop() {
  send_i2c_pkt ();
}


void send_i2c_pkt () {

  i2c_bytes_count = 0;

  randNumber = random(MAX_32_BIT_VALUE);
  Serial.print("Random number: ");
  Serial.println(randNumber, HEX);

  while (i2c_bytes_count < I2C_PKT_LENGTH) {

    for (int i = 0; i < I2C_PKT_LENGTH; i++) {
      //extract the right-most byte of the shifted variable
      splitted_msg[i] = ((randNumber >> (i * 8)) & 0xFF);
    }

    Wire.beginTransmission(I2C_ADDR);
    Wire.write(splitted_msg[i2c_bytes_count]);
    Wire.endTransmission();    // stop transmitting
    i2c_bytes_count ++;
  }
}
