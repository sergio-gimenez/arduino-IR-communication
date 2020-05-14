#define TIMEOUT 4500

int has_tx_started = false;
unsigned int start_time;
boolean isEOT = false;

int rx_amp_pin = 0; // Analog pin for read
int rx_amp;         // Quantized measured value
float v_rx_amp;     // Voltage measured in mV

int rx_preamp_pin = 2;
int rx_preamp;
float v_rx_preamp;

int tx_pin = 1;
int tx;
float v_tx;


void setup() {
  Serial.begin(38400);

  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  //This message can be removed if recording data into file
  //Serial.println("Waiting for an IR packet");
}

void loop() {
  rx_amp = analogRead(rx_amp_pin);
  rx_preamp = analogRead(rx_preamp_pin);
  tx = analogRead(tx_pin);

  // Start program if message received and loop thrpugh it while transmission is not over
  if (((rx_amp > 300) || (tx > 150)) && !isEOT) {
    has_tx_started = true;
    start_time = millis();

    while (has_tx_started && !isEOT) {

      v_rx_amp = convert_to_milivolts(rx_amp);
      v_rx_preamp = convert_to_milivolts(rx_preamp);
      v_tx = convert_to_milivolts(tx);

      print_data_through_serial(v_rx_amp, v_rx_preamp, v_tx);

      Serial.println(millis() - start_time);

      rx_amp = analogRead(rx_amp_pin);
      rx_preamp = analogRead(rx_preamp_pin);
      tx = analogRead(tx_pin);

      // Expected 6 IR messages from raspi
      // That's enough time to the messages to arrive. If they don't, something happened
      if ((millis() - start_time) > TIMEOUT) {
        isEOT = true;

        //This message can be removed if recording data into file
        //Serial.println("EOT");
      }
    }
  }
}

float convert_to_milivolts(int quantized_read_value) {
  return ((float(quantized_read_value)) * 5 / 1024) * 1000;
}

void print_data_through_serial(float v_rx_amp, float v_rx_preamp, float v_tx) {
  // Print data in pseudo-csv format over Serial
  Serial.print(v_rx_amp);
  Serial.print(",");
  Serial.print(v_rx_preamp);
  Serial.print(",");
  Serial.print(v_tx);
  Serial.print(",");
}
