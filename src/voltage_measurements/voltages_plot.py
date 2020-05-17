#!/usr/bin/env python3

import plotter
import matplotlib.pyplot as plt

def main():
    RECORDED_DATA_FILENAME = "out.csv"
    time = []
    v_rx_amp = []
    v_rx_preamp = []
    v_tx = []

    v_rx_amp, v_rx_preamp, v_tx, time = plotter.get_voltages_from_file(RECORDED_DATA_FILENAME)

    plt.plot(time, v_rx_preamp, label="Pre-amplified Rcv V", color='orange')
    plt.plot(time, v_rx_amp, label="Amplified Rcv V", color='cyan')
    plt.plot(time, v_tx, label="V Tx from Transmitter", color='green')

    format_plot('Time [ms]', 'Voltage [mV]',
                'IR Transmission\nReceived 6 messages (32bit each)')

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