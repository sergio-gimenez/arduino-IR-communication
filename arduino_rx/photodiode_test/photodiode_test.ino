
int senRead = 0;               //Readings from sensor to analog pin 0
int has_tx_started = false;
unsigned int start_time;

void setup()
{
  Serial.begin(9600);    //setting serial monitor at a default baund rate of 9600
  
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Waiting for an IR packet");

}

void loop()
{

  int val = analogRead(senRead); //variable to store values from the photodiode

  if (val > 300) {
    has_tx_started = true;
    start_time = millis();    
    
    while (has_tx_started) {
      Serial.print(val);          // prints the values from the sensor in serial monitor
      Serial.print(";");
      Serial.println(millis() - start_time);
      val = analogRead(senRead); //variable to store values from the photodiode
    }
  }
}
