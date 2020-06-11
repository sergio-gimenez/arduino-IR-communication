#include <Wire.h> // I2C library

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
#define I2C_ADDR 8
#define PKT_LENGTH 4 //In bytes
#define ACK 0x06 //ASCII ACK character
#define EXPERIMENT_ITERATIONS 100000

long randNumber;
int tx_buf[4];
byte buf_to_send[4];
int splitted_msg[4];
int i2c_bytes_count;
long iterations_counter = 0;

void setup() {
  Wire.begin();
  Serial.begin(2400);
  get_ir_tx_ready();

  /*
     As analog input pin 0 is unconnected, random analog
     noise will cause the call to randomSeed() to generate
     different seed numbers each time the sketch runs.
     randomSeed() will then shuffle the random function.
  */
  randomSeed(analogRead(0));

  // Delay in order to wait for the receiver
  delay(5000);

  // Send first data frame
  randNumber = random(MAX_32_BIT_VALUE);
  send_i2c_pkt(randNumber);
  Serial.println(randNumber);
}


void loop() {
  if (iterations_counter < EXPERIMENT_ITERATIONS) {
    // Wait for ACK before sending
    if ((Serial.available() > 0) && (Serial.read() == ACK)) {
      randNumber = random(MAX_32_BIT_VALUE);
      send_i2c_pkt(randNumber);
      Serial.println(randNumber);
      iterations_counter++;
    }
  }
}



void send_i2c_pkt (long i2c_randNumber) {
  i2c_bytes_count = 0;

  while (i2c_bytes_count < PKT_LENGTH) {

    for (int i = 0; i < PKT_LENGTH; i++) {
      //extract the right-most byte of the shifted variable
      splitted_msg[i] = ((i2c_randNumber >> (i * 8)) & 0xFF);
    }

    Wire.beginTransmission(I2C_ADDR);
    Wire.write(splitted_msg[i2c_bytes_count]);
    Wire.endTransmission();
    i2c_bytes_count ++;
  }
}



// return TIMER2 TOP value per given desired frequency (Hz)
uint8_t timer2top(unsigned long freq) {
  return ((byte)((unsigned long)SYSCLOCK / 2 / freq) - 1);
}


// Prepare arduino to send modulated UART data.
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
