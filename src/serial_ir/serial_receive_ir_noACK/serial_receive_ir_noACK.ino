
long rcv_msg;
long msg;

void setup() {
  Serial.begin(2400) ;
  Serial.print("\nArduino is ready\n");
}

//void loop() {
//  rcv_msg = Serial.parseInt();
//  if (rcv_msg != 0) {
//    Serial.println("OK");
//  }
//}

void loop() {
  int n, i, ch;

  n = Serial.available();
  if (n > 0) {
    i = n;
    while (i--) {
      ch = Serial.read();
      Serial.print((char)ch);
    }
  }
}
