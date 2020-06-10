byte receivedByte;

void setup() {
  Serial.begin(2400);
}

void loop() {
  if (Serial.available() > 0) {
    receivedByte = Serial.read();
    if(receivedByte == 'A'){
      Serial.write('B');
    }
  }
}
