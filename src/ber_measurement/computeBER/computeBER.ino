#include <IRremote.h>
#include <Wire.h>


#define LAST_IR_MESSAGE_TIMEOUT 5000
#define I2C_PKT_LENGTH 5

// TODO add verbose


// IR VARIABLES \\

int IR_RECV_PIN = 11;
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

// Expected message from the sender (generated randomly)
long expected_message = 0b00000000000000000000000000000000;
long wrong_bits_sum;
int received_msgs_count;
boolean has_tx_started = false;
int last_message_timestamp = 0;


// I2C variables \\

int i2c_bytes_count;
unsigned long i2c_rbuf[5];
unsigned long msg;
boolean isEOT = false;
int offset;
long rcv_i2c_pkts;
boolean isI2CinBuf = false;

boolean isIRinBuf = false; //TODO I think it can be removed

// TODO Remove checksum stuff since it is not needed
int rx_checksum, computed_checksum;
boolean isChecksumComing = false;


void setup()
{
  Serial.begin(38400);

  // In case the interrupt driver crashes on setup, give a clue to the user what's going on.
  irrecv.enableIRIn();

  // join i2c bus with address #8
  Wire.begin(8);

  // register event
  Wire.onReceive(handle_i2c_event);
  
  Serial.println("Enabled IRin and I2C bus");

}

void loop() {

  if (irrecv.decode(&results) && isI2CinBuf) {
    Serial.println("i2c packet is in buffer");
    has_tx_started = true;
    
    Serial.print("IR Received message: ");
    Serial.println(results.value, HEX);
    Serial.println("\n");

    received_msgs_count++;

    // Sum of all wrong bits in the transmission
    wrong_bits_sum += get_wrong_bits(results.value, expected_message);

    last_message_timestamp = millis();

    // Receive the next value
    irrecv.resume();

    isI2CinBuf = false;
  }

  // Check if the timeout is over
  unsigned int time_elapsed = millis() - last_message_timestamp;
  if (((time_elapsed > LAST_IR_MESSAGE_TIMEOUT) && has_tx_started)) {
    
    double BER = wrong_bits_sum / (32 * received_msgs_count);
    
    Serial.print("Average BER for the transmission: ");
    Serial.println(BER, 20);
    Serial.print("Received i2c pckts: ");
    Serial.println(rcv_i2c_pkts / I2C_PKT_LENGTH);
    Serial.print("Total time elapsed: ");
    Serial.println(millis() / (1000 - (LAST_IR_MESSAGE_TIMEOUT/1000)));

    has_tx_started = false;
    time_elapsed = 0;
    received_msgs_count = 0;
    wrong_bits_sum = 0;
  }
}


// Detect the errors in the message received and returns the amount of errors in a message.
// Argumnents:
// - received_msg: The received message you actually want to count the errors.
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


// Function that executes whenever data is received from master
// This function is registered as an event, see setup()
void handle_i2c_event() {

  rcv_i2c_pkts++;

  // Received int
  i2c_rbuf[i2c_bytes_count] = Wire.read();

  // Count for the first 5 bytes since packet length is 5 (msg + cheksum)
  if (i2c_bytes_count < 5) {
    if (!isChecksumComing) {
      msg |= i2c_rbuf[offset / 8] << offset;
    }

    if (i2c_bytes_count == 3) {
      isChecksumComing = true;
    }

    if (isChecksumComing && i2c_bytes_count == 4) {
      rx_checksum = i2c_rbuf[i2c_bytes_count];
      isEOT = true;
    }

    i2c_bytes_count ++;
    offset += 8;
  }

  if (isEOT) {

    // Verify that received message is not a i2c scan
    if (!(msg == 0xFFFFFFFF)) {

      Serial.print("I2C Received message = ");
      Serial.println(msg, HEX);

      for (int i = 0; i < 4; i++) {
        computed_checksum += i2c_rbuf[i];
      }
      computed_checksum = computed_checksum & 0xff;

      if (computed_checksum == rx_checksum) {
        expected_message = msg;
      }
    }

    // End of i2c transmission. Reinitialize variables
    msg = 0;
    i2c_bytes_count = 0;
    offset = 0;
    isEOT = false;
    isChecksumComing = false;
    computed_checksum = 0;
    isI2CinBuf = true;

  }
}
