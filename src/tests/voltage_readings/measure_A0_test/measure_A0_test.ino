
int senRead = 0;               //Readings from sensor to analog pin 0
int has_tx_started = false;
unsigned int start_time;
int expected_messages = 5;
int i = 0;
boolean isEOT = false;
float quantized_voltage;

void setup()
{
  Serial.begin(9600);    //setting serial monitor at a default baund rate of 9600

  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  //This message can be removed if recording data into file
  //Serial.println("Waiting for an IR packet");

}

void loop()
{

  int val = analogRead(senRead); //variable to store values from the photodiode

  if ((val > 300) && !isEOT) {
    has_tx_started = true;
    start_time = millis();

    while (has_tx_started && !isEOT) {
      if (i < 5) {
        quantized_voltage = (float(val) * 5 / 1024) * 1000;
        Serial.print(quantized_voltage);          // prints the values from the sensor in serial monitor
        Serial.print(",");
        Serial.println(millis() - start_time);
        val = analogRead(senRead); //variable to store values from the photodiode

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
}
