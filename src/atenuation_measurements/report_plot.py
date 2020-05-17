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

    max_rx_values = plotter.get_msg_max_amplitude(v_rx_preamp, time)
    max_tx_values = plotter.get_msg_max_amplitude(v_tx, time)

    max_rx_amplitude = plotter.get_avg_voltage_ready_to_plot(max_rx_values, v_rx_preamp, time)
    max_tx_amplitude = plotter.get_avg_voltage_ready_to_plot(max_tx_values, v_tx, time)

    fig, (ax1, ax2) = plt.subplots(2, sharex=True)
    
    ax1.plot(time, v_rx_preamp, label="Pre-amplified Rcv V", color='orange')
    ax1.plot(time, v_tx, label="V Tx from Transmitter", color='green')
    ax2.plot(time, max_rx_amplitude, label="Max amplitude Rx", color='red', linestyle='--')
    ax2.plot(time, max_tx_amplitude, label="Max amplitude Tx", color='darkred', linestyle='--')

    format_plot(fig, ax1, ax2)



def format_plot(fig, ax1, ax2):
    fig.suptitle('6 IR messages transmission')
    plt.xlim(0, 4000)
    ax1.set_xlabel("ms")
    ax1.set_ylabel("mV")
    ax2.set_xlabel("ms")
    ax2.set_ylabel("mV")
    ax1.grid()
    ax2.grid()
    ax1.legend()
    ax2.legend()
    plt.show()

if __name__ == "__main__":
    main()