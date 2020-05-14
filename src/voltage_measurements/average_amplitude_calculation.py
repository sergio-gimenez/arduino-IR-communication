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

    rx_messages = plotter.get_IR_messages(v_rx_preamp, time)[0]
    tx_messages = plotter.get_IR_messages(v_tx, time)[0]
    time_acc = plotter.get_IR_messages(v_tx, time)[1]

    non_zeros_rx = plotter.remove_PWM_gaps(rx_messages)
    non_zeros_tx = plotter.remove_PWM_gaps(tx_messages)

    avg_rx_means = plotter.compute_avg(non_zeros_rx)
    avg_tx_means = plotter.compute_avg(non_zeros_tx)

    avg_rx_amplitude = plotter.get_avg_voltage_ready_to_plot(avg_rx_means, v_rx_preamp, time)
    avg_tx_amplitude = plotter.get_avg_voltage_ready_to_plot(avg_tx_means, v_tx, time)

    plt.plot(time, v_rx_preamp, label="Pre-amplified Rcv V", color='orange')
    plt.plot(time, v_tx, label="V Tx from Transmitter", color='green')
    plt.plot(time, avg_rx_amplitude, label="Avg amplitude Rx", color='red', linestyle='--')
    plt.plot(time, avg_tx_amplitude, label="Avg amplitude Tx", color='darkred', linestyle='--')

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