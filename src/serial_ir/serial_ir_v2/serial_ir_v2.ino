#define cr_pin 9

void setup() {
  Serial.begin(1200);  /* Define baud rate for serial communication */
  tone(cr_pin, 38000);  /* For modulation at 38kHz */
}

void loop() {
  int count;
  for(count = 0; count<100; count++)
  {
    Serial.println(count);
    delay(1000);
  }
}
