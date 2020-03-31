#include <IRremote.h>


// Timeout for the BER computation in order to know the last message
#define timeout 10000

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

// Expected message from the sender (generated randomly)
long expected_message = 0b00000000000000000000000000000000;

// Acumulative sum of the wrong bits of the transmission
long wrong_bits;

// Total number of messages received
int tx_count;

// Timestamp for the last received message
int last_message_timestamp = 0;




long get_wrong_bits(long expected_msg, long received_msg);




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
    Serial.print("Received message: ");
    Serial.println(results.value, BIN);

    tx_count++;

    // Sum of all wrong bits in the transmission
    wrong_bits += get_wrong_bits(results.value, expected_message);

    Serial.print("Number of wrong bits: ");
    Serial.println(wrong_bits);
    Serial.print("Number of messages received: ");
    Serial.println(tx_count);

    // Start the timeout
    last_message_timestamp = millis();

    irrecv.resume(); // Receive the next value
  }

  // Check if the timeout is over

  //  Serial.print("timestamp - millis = ");
  //  Serial.println(aux);
  //  Serial.print("timestamp = ");
  //  Serial.println(last_message_timestamp);
  //  Serial.print("timeout = ");
  //  Serial.println(timeout);

  unsigned int time_elapsed = millis() - last_message_timestamp;
  Serial.print("Time elapsed = ");
  Serial.println(time_elapsed);
  if (time_elapsed > timeout) {

    Serial.println("Transmission is over!");
    time_elapsed = 0;
  }
  delay(100);
}


// Detect the errors in the message received and returns the amount of errors in a message.
// Argumnents:
// - input_message: The received message you actually want to count the errors.
// - expected_message: The expected received message
long get_wrong_bits(long expected_msg, long received_msg)
{
  long count = 0;

  // The messages are 32 bits max.
  for (int i = 0; i < 32; i++) {

    // right shift both the numbers by 'i' and
    // check if the bit at the 0th position is different
    if (((expected_msg >> i) & 1) != ((received_msg >> i) & 1)) {
      count++;
    }
  }

  return count;

}
