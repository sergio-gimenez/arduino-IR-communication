int state = 0;

void setup() {
  Serial.begin(2400);
  pinMode(7, INPUT);    // sets the digital pin 7 as input
}

void loop() {
  if(digitalRead(7) == HIGH){
    Serial.println("I'm HIGH");  
  }
  else if (digitalRead(7) == LOW){
    Serial.println("I'm LOW");
  }
}
