
long rcv_msg;

void setup() {
  Serial.begin(2400) ;
  Serial.print("\nArduino is ready\n");
}

void loop() {
  rcv_msg = Serial.parseInt();
  if (rcv_msg != 0) {
    //Serial.println(rcv_msg);
    Serial.write('A');
  }
}
