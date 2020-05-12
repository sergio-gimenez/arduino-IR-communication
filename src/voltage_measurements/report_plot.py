#!/usr/bin/env python3

import plotter
import matplotlib.pyplot as plt

RECORDED_DATA_FILENAME = "out.csv"
time = []
v_rx_amp = []
v_rx_preamp = []
v_tx = []

v_rx_amp, v_rx_preamp, v_tx, time = plotter.get_voltages_from_file(RECORDED_DATA_FILENAME)

max_rx_values = plotter.get_msg_max_amplitude(v_rx_preamp, time)
max_tx_values = plotter.get_msg_max_amplitude(v_tx, time)

max_rx_amplitude = plotter.get_avg_voltage_ready_to_plot(max_rx_values, v_rx_preamp, time)
max_tx_amplitude = plotter.get_avg_voltage_ready_to_plot(max_tx_values, v_tx, time)

plt.plot(time, v_rx_preamp, label="Pre-amplified Rcv V", color='orange')
plt.plot(time, v_tx, label="V Tx from Transmitter", color='green')
plt.plot(time, max_rx_amplitude, label="Max amplitude Rx", color='red', linestyle='--')
plt.plot(time, max_tx_amplitude, label="Max amplitude Tx", color='darkred', linestyle='--')

plotter.format_plot('Time [ms]', 'Voltage [mV]',
            'IR Transmission\nReceived 6 messages (32bit each)')

