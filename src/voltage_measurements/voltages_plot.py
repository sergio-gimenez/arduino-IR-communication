#!/usr/bin/env python3

import plotter
import matplotlib.pyplot as plt

RECORDED_DATA_FILENAME = "out.csv"

time = []
v_rx_amp = []
v_rx_preamp = []
v_tx = []

plotter.get_voltages_from_file(RECORDED_DATA_FILENAME, v_rx_amp, v_rx_preamp, v_tx, time)

plt.plot(time, v_rx_amp, label="Amplified Rcv V", color='cyan')
plt.plot(time, v_rx_preamp, label="Pre-amplified Rcv V", color='orange')
plt.plot(time, v_tx, label="V Tx from Transmitter", color='green')

plotter.format_plot('Time [s]', 'Voltage [mV]',
                'IR Transmission\nReceived 6 messages (32bit each)')

