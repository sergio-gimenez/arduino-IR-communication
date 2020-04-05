#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "irslinger.h"

#define RAND_MAX_32B 4294967296

int main(int argc, char *argv[])
{

    // ** Random message generation ** \\

    // The random number is seeded with the current time.
    srand(time(0));
    char rand_msg[32];

    int num = (rand() % (RAND_MAX_32B + 1));

    for (int i = 0; i < 32; i++){
    // Right shift num, n times and perform bitwise AND with 1
    int current_bit = (num >> i) & 1;

    // Add the i-bit to the i-position of the array
    sprintf(&rand_msg[i], "%d",current_bit);
    }

    printf("Random message generated: %s\n\n", rand_msg);

    // Write generated member in file
    FILE *file;
    file = fopen("expected_message","w");
    fprintf(file, "%u", rand_msg);
    fclose(file);

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

    return 0;
}

