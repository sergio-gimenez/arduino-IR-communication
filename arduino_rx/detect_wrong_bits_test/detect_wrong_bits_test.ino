long A = 0b11111111111111111111111111111111; 
long B = 0b00000000000000000000000000000000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // open the serial port at 9600 bps:
  
  int wrong_bits;
  wrong_bits = get_wrong_bits(A, B);

  Serial.print("Number of wrong bits: ");
  Serial.println(wrong_bits);
}

void loop() {

}

// Detect the errors in the message received and returns the amount of errors in a message.
// Argumnents:
// - input_message: The received message you actually want to count the errors.
// - expected_message: The expected received message
int get_wrong_bits(long expected_msg, long received_msg)
{
  int count = 0;

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
