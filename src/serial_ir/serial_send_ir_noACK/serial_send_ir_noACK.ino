// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define SYSCLOCK 16000000  // main system clock (Hz)
#define PULSECLOCK 38000  // Hz
#define IROUT 11
#define MAX_32_BIT_VALUE 2147483647
#define PKT_LENGTH 4 //In bytes

long randNumber;
int tx_buf[4];
byte buf_to_send[4];
boolean ACK_received = false;

uint8_t timer2top(unsigned long freq);
void get_ir_tx_ready();

void setup() {
  Serial.begin(2400);
  get_ir_tx_ready();

  /*
     If analog input pin 0 is unconnected, random analog
     noise will cause the call to randomSeed() to generate
     different seed numbers each time the sketch runs.
     randomSeed() will then shuffle the random function.
  */
  randomSeed(analogRead(0));

  // Delay in order to wait for the receiver
  delay(5000);
}

int i = 0;
long time_elapsed;
long start_timer;

void loop() {
  if ( i == 0) {
    start_timer = millis();
  }
  if (i < 200) {
    //randNumber = random(MAX_32_BIT_VALUE);
    Serial.println("AAAA");
    i++;
  }
  if ( i == 200) {
    time_elapsed = millis() - start_timer;
    Serial.print("Total time elapsed: ");
    Serial.println(time_elapsed);
    Serial.println(" seconds");
    i++;
  }
}


// return TIMER2 TOP value per given desired frequency (Hz)
uint8_t timer2top(unsigned long freq) {
  return ((byte)((unsigned long)SYSCLOCK / 2 / freq) - 1);
}


void get_ir_tx_ready() {
  cbi(TCCR2A, COM2A1); // connect OC2A (COM2A0 = 1)
  sbi(TCCR2A, COM2A0);

  cbi(TCCR2B, WGM22); // CTC mode for TIMER2
  sbi(TCCR2A, WGM21);
  cbi(TCCR2A, WGM20);

  TCNT2 = 0;

  cbi(ASSR, AS2); // use system clock for timer 2

  OCR2A = 255;   // set TOP to 255 for now

  cbi(TCCR2B, CS22); // TIMER2 prescale = 1
  cbi(TCCR2B, CS21);
  sbi(TCCR2B, CS20);

  cbi(TCCR2B, FOC2A); // clear forced output compare bits
  cbi(TCCR2B, FOC2B);

  pinMode(IROUT, OUTPUT);  // set OC2A to OUPUT
  OCR2A = timer2top(PULSECLOCK);
  sei();
}
