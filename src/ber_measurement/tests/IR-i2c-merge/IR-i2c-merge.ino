/*
   IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/
#include <Wire.h>
#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;


int i2c_bytes_count;

//unsigned long rbuf[4] = {170, 170, 170, 170};
unsigned long rbuf[5];

int rx_checksum, computed_checksum;

unsigned long msg;

boolean isEOT = false;

boolean isChecksumComing = false;

int offset;

long total_i2c_pkts = 0;


void setup()
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("IR & I2C receiver ready");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("IR Received message (HEX): ");
    Serial.println(results.value, HEX);
    Serial.print("IR Received message (BIN): ");
    Serial.println(results.value, BIN );
    Serial.print("\n");
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  total_i2c_pkts++;

  // Received int
  rbuf[i2c_bytes_count] = Wire.read();
//  Serial.print("Received byte ");
//  Serial.println(i2c_bytes_count);
//  Serial.println(rbuf[i2c_bytes_count]);

  // Count for the first 5 bytes since packet length is 5 (msg + cheksum)
  if (i2c_bytes_count < 5) {
    if (!isChecksumComing) {
      msg |= rbuf[offset / 8] << offset;

//      Serial.print("offset =");
//      Serial.println(offset);
//
//      Serial.print("offset/8 =");
//      Serial.println(offset / 8);
    }

    if (i2c_bytes_count == 3) {
      isChecksumComing = true;
    }

    if (isChecksumComing && i2c_bytes_count == 4) {
      rx_checksum = rbuf[i2c_bytes_count];
      isEOT = true;
    }

    i2c_bytes_count ++;
    offset += 8;

  } if (isEOT) {
    Serial.print("Received message = ");
    Serial.println(msg, HEX);
    Serial.println("\n");
//    Serial.print("\nReceived checksum is: ");
//    Serial.println(rx_checksum);

    for (int i = 0; i < 4; i++) {
      computed_checksum += rbuf[i];
    }
    computed_checksum = computed_checksum & 0xff;
    //Serial.print("Computed checksum is: ");
    //Serial.println(computed_checksum);

    if (computed_checksum == rx_checksum) {
     // Serial.println("Message is correct\n");
    }
    msg = 0;
    i2c_bytes_count = 0;
    offset = 0;
    isEOT = false;
    isChecksumComing = false;
    computed_checksum = 0;
    Serial.print("Total i2c packets = ");
    Serial.println(total_i2c_pkts/5);
  }
}
