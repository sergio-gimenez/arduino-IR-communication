
#include <Wire.h>

#define I2C_ADDR 8
#define MAX_32_BIT_VALUE 2147483647

long randNumber;

void setup() {
  Serial.begin(2400);
  Wire.begin();
}
void loop() {
  randNumber = random(MAX_32_BIT_VALUE);

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(randNumber);
  Wire.endTransmission();    // stop transmitting
  Serial.print("Transmitted number: ");
  Serial.println(randNumber);
  delay(1000);
}
