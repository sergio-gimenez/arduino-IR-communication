int inPin = 7;    // pushbutton connected to digital pin 7
int val = 0;      // variable to store the read value

void setup() {
  pinMode(inPin, INPUT);    
  Serial.begin(115200);
}

void loop() {
  val = digitalRead(inPin); 
  Serial.println(val);
}
