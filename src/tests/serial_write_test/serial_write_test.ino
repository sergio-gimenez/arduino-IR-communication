byte receivedByte;

void setup() {
  Serial.begin(2400);
  Serial.write('A');
}

void loop() {
  if (Serial.available() > 0) {
    receivedByte = Serial.read();
    if(receivedByte == 'B'){
      Serial.write('C');
    }
  }
}
