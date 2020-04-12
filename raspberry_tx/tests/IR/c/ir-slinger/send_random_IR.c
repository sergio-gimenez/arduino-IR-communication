#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "irslinger.h"
 
#define MAXCHAR 33

int main() {

     // ** Get random message previously generated from file ** \\

    FILE *fp;
    char rand_msg[MAXCHAR];
    char* filename = "expected_message.txt";
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    fgets(rand_msg, MAXCHAR, fp);
    fclose(fp);

    // ** IR pulse generation ** \\

    uint32_t outPin = 23;            // The Broadcom pin number the signal will be sent on
    int frequency = 38000;           // The frequency of the IR signal in Hz
    double dutyCycle = 0.5;          // The duty cycle of the IR signal. 0.5 means for every cycle,
                                     // the LED will turn on for half the cycle time, and off the other half
    int leadingPulseDuration = 9000; // The duration of the beginning pulse in microseconds
    int leadingGapDuration = 4500;   // The duration of the gap in microseconds after the leading pulse
    int onePulse = 562;              // The duration of a pulse in microseconds when sending a logical 1
    int zeroPulse = 562;             // The duration of a pulse in microseconds when sending a logical 0
    int oneGap = 1688;               // The duration of the gap in microseconds when sending a logical 1
    int zeroGap = 562;               // The duration of the gap in microseconds when sending a logical 0
    int sendTrailingPulse = 1;       // 1 = Send a trailing pulse with duration equal to "onePulse"
                                     // 0 = Don't send a trailing pulse
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
        rand_msg);

    printf("IR message sent is %s\n\n", rand_msg);
    return 0;
}
