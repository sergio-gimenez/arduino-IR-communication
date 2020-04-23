int has_tx_started = false;
unsigned int start_time;
boolean isEOT = false;

int rx_amp_pin = 0; // Analog pin for read
int rx_amp;         // Quantized measured value
float v_rx_amp;     // Voltage measured in mV

int rx_preamp_pin = 2;
int rx_preamp;
float v_rx_preamp;

void setup() {
  Serial.begin(9600);

  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  //This message can be removed if recording data into file

  //Serial.println("Waiting for an IR packet");
}

void loop() {
  rx_amp = analogRead(rx_amp_pin);
  rx_preamp = analogRead(rx_preamp_pin);

  // Start program if message received and while transmission is not over
  if ((rx_amp > 300) && !isEOT) {
    has_tx_started = true;
    start_time = millis();

    while (has_tx_started && !isEOT) {

      // TODO program function
      // Convert quantized value measured from arduino to mV
      v_rx_amp = (float(rx_amp) * 5 / 1024) * 1000;
      v_rx_preamp = (float(rx_preamp) * 5 / 1024) * 1000;

      // TODO program function
      // Print stuff in pseudo-csv format
      Serial.print(v_rx_amp);
      Serial.print(",");
      Serial.print(v_rx_preamp);
      Serial.print(",");
      Serial.println(millis() - start_time);

      rx_amp = analogRead(rx_amp_pin);
      rx_preamp = analogRead(rx_preamp_pin);

      // Expected 6 IR messages from raspi
      // That's enough time to the messages to arrive. If they don't, something happened
      if ((millis() - start_time) > 4500) {
        isEOT = true;

        //This message can be removed if recording data into file
        //Serial.println("EOT");
      }
    }
  }
}
