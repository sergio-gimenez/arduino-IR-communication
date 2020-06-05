
void setup() {
  Serial.begin(2400) ;
}

void loop() {
  int bytes_to_read_in_buffer, i, current_byte;

  bytes_to_read_in_buffer = Serial.available();
  if (bytes_to_read_in_buffer > 0) {
    i = bytes_to_read_in_buffer;
    while (i--) {
      current_byte = Serial.read();
      Serial.print((char) current_byte);
    }
  }
}
