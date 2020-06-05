
void setup() {
 Serial.begin(2400) ;
}

void loop() {
 int n, i, ch ;
 
 n = Serial.available() ;
 if (n > 0) {
       i = n ;
   while (i--) {
     ch = Serial.read() ;
     Serial.print((char)ch) ;
   }
 }
}
