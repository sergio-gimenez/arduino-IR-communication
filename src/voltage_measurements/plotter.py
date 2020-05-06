#!/usr/bin/env python3

import matplotlib.pyplot as plt
import sys

RECORDED_DATA_FILENAME = "out.csv"

def main():
    time = []
    v_rx_amp = []
    v_rx_preamp = []
    v_tx = []    

    v_rx_amp, v_rx_preamp, v_tx, time = get_voltages_from_file(RECORDED_DATA_FILENAME)

    #import pdb; pdb.set_trace()

    plt.plot(time, v_rx_amp, label="Amplified Rcv V")
    plt.plot(time, v_rx_preamp, label="Pre-amplified Rcv V")
    plt.plot(time, v_tx, label="V Tx from Transmitter")

    format_plot('Time [s]', 'Voltage [mV]',
                'IR Transmission\nReceived 6 messages (32bit each)')


def get_voltages_from_file(output_filename):
    # Read the file and save it in an array without the \n character
    # Each position has: [v_rx_amp, v_rx_preamp, v_tx, t]
    records = open(output_filename, "r").read().splitlines()
    return delete_wrong_records(records)


def format_plot(x_axis_label, y_axis_label, title):
    plt.xlabel(x_axis_label)
    plt.ylabel(y_axis_label)
    plt.title(title)
    plt.xlim(0, 3500)
    plt.ylim(0, 5000)
    plt.grid()
    plt.legend()
    plt.show()


def check_if_record_is_correct(str_v_rx_amp, str_v_rx_preamp, str_v_tx, str_time):
    pass     

def delete_wrong_records(records):    
    v_rx_amp = []
    v_rx_preamp = []
    v_tx = []
    time = []
    wrongs = []
    
    for record in records:
        # If the format is not [float, float, float, int] discard the record
        try:
            aux_v_rx_amp = float(record.split(",")[0])
            aux_v_rx_preamp = float(record.split(",")[1])
            aux_v_tx = float(record.split(",")[2])
            aux_time = int(record.split(",")[3])

            v_rx_amp.append(aux_v_rx_amp)
            v_rx_preamp.append(aux_v_rx_preamp)
            v_tx.append(aux_v_tx)
            time.append(aux_time)

        except (ValueError, IndexError, UnicodeDecodeError):
            wrongs.append(record)

            print("Some value could not be properly read from the output file.",
                 "Maybe the baudrate is not properly set?")
            continue

    return v_rx_amp, v_rx_preamp, v_tx, time

if __name__ == "__main__":
    main()