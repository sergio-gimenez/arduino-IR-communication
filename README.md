# IR communication 

This is the very simple scenario I have so far. I have managed to send a 32 bits IR signal from a transmitter to a receiver.

![global_diagram](/docs/img/follow-up-23-3-diagram.png)

## Set-up 

This is the whole simple-system schema:
![global_schema](/docs/img/follow-up-23-03-schema_bb.png)



### Transmitter

The emitter is based on a **Raspberry Pi 2 model v1.1** and an **940 nm IR LED**, plus a **330 ohm resistor** to limit de current.

![tx_raspberry](/docs/img/Tx_raspberry.png)

It can send a maximum (as far as I know) of 32 bits through a 38 kHz signal. But I encountered some things to deal with:

* Send the pulses in a consistent 38 kHz signal.

* Avoid the OS running in the raspberry to interrupt the signal generation in the GPIO pins.

In order to solve the issues, I have used two libraries:

* [pigpio](http://abyz.me.uk/rpi/pigpio/). This library  allows to construct arbitrary waveforms to give precise timing of the output GPIO level changes (accurate to a few microseconds). This means that it will build up the infrared wave ahead of time and hand it off to be transmitted in a kernel extension where it won't be interrupted. So the interruption problem is solved. 

* [ir-slinger](https://github.com/bschwind/ir-slinger). This library is a wrapper for pigpio library, which makes really easy to use the ton of funcionalities that pigpio offers. The code gets really readable and clean with this wrapper.

This is how the code looks. It is written in C.

```c
#include <stdio.h>
#include "irslinger.h"

int main(int argc, char *argv[])
{
    uint32_t outPin = 23;            // The Broadcom pin number the signal will be sent on
    int frequency = 38000;           // The frequency of the IR signal in Hz
    double dutyCycle = 0.5;          // The duty cycle of the IR signal. 0.5 means for every cycle,
                                     // the LED will turn on for half the cycle time, and off the other $
    int leadingPulseDuration = 9000; // The duration of the beginning pulse in microseconds
    int leadingGapDuration = 4500;   // The duration of the gap in microseconds after the leading pulse
    int onePulse = 562;              // The duration of a pulse in microseconds when sending a logical 1
    int zeroPulse = 562;             // The duration of a pulse in microseconds when sending a logical 0
    int oneGap = 1688;               // The duration of the gap in microseconds when sending a logical 1
    int zeroGap = 562;               // The duration of the gap in microseconds when sending a logical 0
    int sendTrailingPulse = 1;       // 1 = Send a trailing pulse with duration equal to "onePulse"
                                     // 0 = Don't send a trailing pulse
    while (1)
    {
        int result = irSling(
        outPin,
        frequency,
        dutyCycle,
        leadingPulseDuration,
        leadingGapDuration,
        onePulse,
        zeroPulse,
        oneGap,
        zeroGap,
        sendTrailingPulse,
        "11111111111111110000000000000000");
    }
    return 0;
}
```

For the parameters of the pulse, I just took the most common values and it worked out.

### Receiver

For the receiver, I use an **Arduino Uno Rev3** that Filippo borrowed me and a chinese **IR receiver (1838D)** that I bought in a shop close to Aristotle Square:

![arduino_rx](/docs/img/Rx_arduino.png)

For receiving signals I use a very complete library and easy to use called [IRremote](https://github.com/z3t0/Arduino-IRremote). I achieved to receive signals simply by running the example code that the library offers and it worked. Here is the code:

```c
/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Recv HEX value:");
    Serial.println(results.value, HEX);
    Serial.print("Recv BIN value:");
    Serial.println(results.value, BIN);
    
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
``