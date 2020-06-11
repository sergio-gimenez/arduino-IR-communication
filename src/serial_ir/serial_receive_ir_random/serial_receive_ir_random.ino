#define PKT_LENGTH 4 //In bytes
#define EXPERIMENT_ITERATIONS 200


byte rbuf[4];
long count = 0;
long start_timer;
boolean start_of_tx = false;

union {
  byte asBytes[4];
  long asLong;
} msg;

void setup() {
  Serial.begin(2400) ;
  Serial.print("\nArduino is ready\n");
}


void loop() {
  if (Serial.available() >= PKT_LENGTH) {

    if(!start_of_tx) start_timer = millis();
    start_of_tx = true;
    
    for (int i = 0; i < PKT_LENGTH; i++) {
      msg.asBytes[i] = (byte)Serial.read();
    }
    Serial.println(msg.asLong);
    //Serial.println(count);
    count++;

  }
  if (count == EXPERIMENT_ITERATIONS) {
    Serial.print("Time elapsed: ");
    Serial.println(millis() - start_timer);
    count++;
  }
}
