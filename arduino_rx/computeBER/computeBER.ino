#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

// Expected message from the sender (generated randomly)
long expected_message = 0b00000000000000000000000000000000;

// Acumulative sum of the wrong bits of the transmission
long wrong_bits;


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

    // Sum of all wrong bits in the transmission
    wrong_bits += get_wrong_bits(results.value, expected_message);

    Serial.print("Number of wrong bits: ");
    Serial.println(wrong_bits);


    irrecv.resume(); // Receive the next value
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
