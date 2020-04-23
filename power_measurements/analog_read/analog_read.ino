// A0 = Amplified Rx
// A1 = Tx
// A2 = Rx


int senRead = 2;
unsigned int start_time;

void setup() {
  Serial.begin(9600);
}


void loop() {

  int val = analogRead(senRead);
  Serial.println(val);
  start_time = millis();

  while (true) {
    Serial.print(val);
    Serial.print(";");
    Serial.println(millis() - start_time);
    val = analogRead(senRead);
  }
}
