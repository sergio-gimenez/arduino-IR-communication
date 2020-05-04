#!/usr/bin/env python3

import matplotlib.pyplot as plt
import sys

RECORDED_DATA_FILENAME = "out.csv"

time = []
v_rx_amp = []
v_rx_preamp = []
v_tx = []

def main():

    get_records_from_file(RECORDED_DATA_FILENAME)

    plt.plot(time, v_rx_amp, label="Amplified Rcv V")
    plt.plot(time, v_rx_preamp, label="Pre-amplified Rcv V")
    plt.plot(time, v_tx, label="V Tx from Transmitter")

    format_plot('Time [s]', 'Voltage [mV]',
                'IR Transmission\nReceived 6 messages (32bit each)')


def get_records_from_file(output_filename):
    # Read the file and save it in an array without the \n character
    # Each position has: [v_rx_amp, v_rx_preamp, v_tx, t]
    records = open(output_filename, "r").read().splitlines()
    for record in records:
        try:
            v_rx_amp.append(float(record.split(",")[0]))
            v_rx_preamp.append(float(record.split(",")[1]))
            v_tx.append(float(record.split(",")[2]))
            time.append(int(record.split(",")[3]))
        except (ValueError, IndexError):
            print("Some value could not be properly read from the output file.",
                  "Maybe the baudrate is not properly set?")
            sys.exit(1)


def format_plot(x_axis_label, y_axis_label, title):
    plt.xlabel(x_axis_label)
    plt.ylabel(y_axis_label)
    plt.title(title)
    plt.xlim(0, 3500)
    plt.ylim(0, 5000)
    plt.grid()
    plt.legend()
    plt.show()

if __name__ == "__main__":
    main()