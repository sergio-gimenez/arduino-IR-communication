/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(38400);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("IR receiver ready");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Received message (HEX): ");
    Serial.println(results.value, HEX);
    Serial.print("Received message (BIN): ");
    Serial.println(results.value, BIN);    
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
