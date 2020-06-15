#include <Wire.h> // I2C library

#define LAST_IR_MESSAGE_TIMEOUT 5000 // millisecons
#define PKT_LENGTH 4 // In bytes
#define I2C_BUS_ADDRESS 8
#define ACK 0x06 //ASCII ACK character 
#define IR_MESSAGE_TIMEOUT 50 //milliseconds
long global_timer_start;
boolean verbose = false;

// IR VARIABLES \\
byte rbuf[4];
long expected_message; // Coming from the sender (randomly generated)
unsigned long wrong_bits_sum;
unsigned long received_msgs_count;
boolean has_tx_started = false;
long last_message_timestamp = 0;
union {
  byte asBytes[4];
  long asLong;
} msg;

// I2C variables \\

int i2c_bytes_count;
unsigned long i2c_rbuf[PKT_LENGTH];
unsigned long i2c_msg;
boolean isEOT = false;
int offset;
unsigned long rcv_i2c_pkts;
boolean isI2CinBuf = false;

void setup()
{
  Serial.begin(2400);
  Wire.begin(I2C_BUS_ADDRESS);
  // register event
  Wire.onReceive(handle_i2c_event);

  // In case the interrupt driver crashes on setup,
  // give a clue to the user what's going on.
  Serial.print("\nArduino is ready\n");
}

void loop() { 
  // Get IR message only if an I2C pkt has been previously received
  if (isI2CinBuf) {
    isI2CinBuf = false;    


    start_timer();
    read_serial_IR_msg();
    received_msgs_count++;

    // Sum of all wrong bits in the transmission
    wrong_bits_sum += get_wrong_bits(expected_message, msg.asLong);
    Serial.println(wrong_bits_sum);

    last_message_timestamp = millis();

    Serial.write(ACK);

  }


  // Check if the timeout is over
  unsigned int time_elapsed = millis() - last_message_timestamp;
  if (((time_elapsed > LAST_IR_MESSAGE_TIMEOUT) && has_tx_started)) {

    print_experiment_report();

    // End of experiment, reinitialize variables.
    has_tx_started = false;
    time_elapsed = 0;
    received_msgs_count = 0;
    wrong_bits_sum = 0;
  }
}

void start_timer() {
  if (!has_tx_started) {
    global_timer_start = millis();
  }
  has_tx_started = true;
}

// Read 4 bytes of the serial buffer.Blocking method until timeout.
void read_serial_IR_msg() {
  long serial_ir_msg_start_time = millis();
  while ((millis() - serial_ir_msg_start_time) < IR_MESSAGE_TIMEOUT) {
    if (Serial.available() >= PKT_LENGTH) {
      for (int i = 0; i < PKT_LENGTH; i++) {
        msg.asBytes[i] = (byte)Serial.read();
      }
      //Debugging prints:
      if (verbose) {
        //Serial.print(" IR Received message = ");
        Serial.print("IR|");
        Serial.println(msg.asLong, HEX);
        Serial.println("\n");
      }
      break;
    }
  }
}

// Detect the errors in the message received and returns the
// amount of errors in a message.
// Argumnents:
// - received_msg: The received message you actually want to
//                 count the errors.
// - expected_message: The expected received message
long get_wrong_bits(long expected_msg, long received_msg)
{
  long count = 0;

  // The messages are 32 bits max.
  for (int i = 0; i < 32; i++) {

    // right shift both the numbers by 'i' and
    // check if the bit at the ith position is different
    if (((expected_msg >> i) & 1) != ((received_msg >> i) & 1)) {
      count++;
    }
  }
  return count;
}


// This function get 4 bytes from the I2C channel and saves them
// in expected_message.
// Function that executes whenever data is received from master
// This function is registered as an event, see setup().
void handle_i2c_event() {

  rcv_i2c_pkts++;

  i2c_rbuf[i2c_bytes_count] = Wire.read();

  if (i2c_bytes_count < PKT_LENGTH) {

    i2c_msg |= i2c_rbuf[offset / 8] << offset;

    if (i2c_bytes_count == (PKT_LENGTH - 1)) {
      isEOT = true;
    }

    i2c_bytes_count ++;
    offset += 8;
  }

  if (isEOT) {

    // Debugging prints
    if (verbose) {
      //Serial.print("I2C Received message = ");
      Serial.print("I2|");
      Serial.println(i2c_msg, HEX);
    }

    expected_message = i2c_msg;

    // End of i2c transmission. Reinitialize variables
    i2c_msg = 0;
    i2c_bytes_count = 0;
    offset = 0;
    isEOT = false;
    isI2CinBuf = true;
  }
}

void print_experiment_report() {

  double BER = (double) wrong_bits_sum / (double)(32 * received_msgs_count);

  //TODO wrap all the prints in a function

  Serial.println("\n\nEXPERIMENT REPORT");
  Serial.println("-------------------\n");

  Serial.print("\n- Total wrong bits in the transmission: ");
  Serial.println(wrong_bits_sum);

  Serial.print("\n- Total bits received: ");
  long total_bits_received =  received_msgs_count * 32;
  Serial.println(total_bits_received);

  Serial.print("\n- Average BER for the transmission: ");
  Serial.println(BER, 20);

  Serial.print("\n- Total time elapsed: ");
  Serial.print((last_message_timestamp - global_timer_start));
  Serial.println(" milliseconds");

  Serial.print("\n\nDuring the whole transmission, received ");
  Serial.print(rcv_i2c_pkts / PKT_LENGTH);
  Serial.print(" i2c pacekts and ");
  Serial.print(received_msgs_count);
  Serial.println(" IR packets\n\n");

}
