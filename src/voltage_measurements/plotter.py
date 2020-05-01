#!/usr/bin/env python3

import matplotlib.pyplot as plt
import sys

time = []
v_rx_amp = []
v_rx_preamp = []
v_tx = []

# Read the file and save it in an array without the \n character
# Each position has: [v_rx_amp, v_rx_preamp, v_tx, t]
out = open("out.csv", "r").read().splitlines()
for i in out:
    try:
        v_rx_amp.append(float(i.split(",")[0]))
        v_rx_preamp.append(float(i.split(",")[1]))
        v_tx.append(float(i.split(",")[2]))
        time.append(int(i.split(",")[3]))
    except (ValueError, IndexError):
        print("Some value could not be properly read from the output file.",
              "Maybe the baudrate is not properly set?")
        sys.exit(1)

# Plot formatting
plt.plot(time, v_rx_amp, label="Amplified Rx V")
plt.plot(time, v_rx_preamp, label="Pre-amplified Rx V")
plt.plot(time, v_tx, label="Tx V")

plt.xlabel('Time [s]')
plt.ylabel('Voltage [mV]')
plt.title('IR Transmission\nReceived 6 messages (32bit each)')
plt.xlim(0, 3500)
plt.ylim(0, 5000)
plt.grid()
plt.legend()
plt.show()