int has_tx_started = false;
unsigned int start_time;
int expected_messages = 5;
int i = 0;
boolean isEOT = false;


int rx_amp_pin = 0;
int rx_amp;
float v_rx_amp;

int rx_preamp_pin = 2;
int rx_preamp;
float v_rx_preamp;

void setup()
{
  Serial.begin(9600);

  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  //This message can be removed if recording data into file

  //Serial.println("Waiting for an IR packet");

}

void loop()
{

  rx_amp = analogRead(rx_amp_pin);
  rx_preamp = analogRead(rx_preamp_pin);


  if ((rx_amp > 300) && !isEOT) {
    has_tx_started = true;
    start_time = millis();

    while (has_tx_started && !isEOT) {
      v_rx_amp = (float(rx_amp) * 5 / 1024) * 1000;
      v_rx_preamp = (float(rx_preamp) * 5 / 1024) * 1000;

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
