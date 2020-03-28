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

//long int expected_input = 0b11111111111111110000000000000000;
long int expected_input =0;
int sum_errors = 0;

int rcv_messages;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");

  
}

void loop() {
  if (irrecv.decode(&results)) {

    //float input_receiver_voltage = analogRead(A5) * (5.0 / 1023.0);
    //Serial.print("Received voltage: ");
    //Serial.println(input_receiver_voltage);
    
    Serial.print("Recv HEX value:");
    Serial.println(results.value, HEX);
    Serial.print("Recv BIN value:");
    Serial.println(results.value, BIN);

    sum_errors += computeBER(results.value);
      
    irrecv.resume(); // Receive the next value
  }

  Serial.println("Computed BER is: " + sum_errors/rcv_messages);  
   
  delay(100);
}

int computeBER(int current_input){

  int current_bit, wrong_bits_count;
  
  for (int i = 0; i < 32; i++){
    current_bit = bitRead(current_input, i);
    if (current_bit != bitRead(expected_input, i)){
      wrong_bits_count++;
    }    
  }
  return wrong_bits_count/32;
}
