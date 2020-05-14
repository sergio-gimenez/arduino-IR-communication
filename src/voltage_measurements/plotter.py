#!/usr/bin/env python3

import matplotlib.pyplot as plt
import sys

MSG_NOISE_THRESHOLD = 150  # In mV
MSG_DURATION = 85.5  # In milliseconds


def main():
    pass

def get_voltages_from_file(output_filename):
    # Read the file and save it in an array without the \n character
    records = open(output_filename, "r").read().splitlines()
    return delete_wrong_records(records)


def check_if_record_is_correct(str_v_rx_amp, str_v_rx_preamp, str_v_tx, str_time):
    pass


def delete_wrong_records(records):
    v_rx_amp = []
    v_rx_preamp = []
    v_tx = []
    time = []
    wrongs = []

    for record in records:
        # Each position has: [v_rx_amp, v_rx_preamp, v_tx, t]
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
            print("Some recorded value was not properly formatted. However, an attempt has been made to process the data despite wrong recordings.",
                  "\nMaybe the baudrate is not properly set or too high?")
            continue

    return v_rx_amp, v_rx_preamp, v_tx, time


def get_IR_messages(IR_transmission, time):

    IR_msg = []
    time_msg = []

    IR_messages = []
    time_messages = []

    start_of_messages = []

    i = 0
    while i < (len(time)):

        # Remove noise and only get IR messages
        if (IR_transmission[i] > MSG_NOISE_THRESHOLD):
            start_of_msg = i
            start_of_messages.append(start_of_msg)

            while(i < (start_of_msg + MSG_DURATION)):
                IR_msg.append(IR_transmission[i])
                time_msg.append(time[i])
                i += 1

            IR_messages.append(IR_msg)
            time_messages.append(time_msg)

            # Remove recorded message because it is already saved in rx_messages.
            IR_msg = []
            time_msg = []

            continue

        i += 1

    return IR_messages, time_messages, start_of_messages


def remove_PWM_gaps(IR_messages):

    no_zeros_msg = []
    no_zeros_messages = []

    for msg in IR_messages:
        for value in msg:
            if(value > MSG_NOISE_THRESHOLD):
                no_zeros_msg.append(value)

        no_zeros_messages.append(no_zeros_msg)
        # Delete values in no_zeros_msg since they are already stored in no_zeros_messages
        no_zeros_msg = []

    return no_zeros_messages


def compute_avg(no_zeros_msgs):
    sum = 0
    means = []
    for msg in no_zeros_msgs:
        for value in msg:
            sum += value

        means.append(sum / len(msg))
        sum = 0

    return means

def get_avg_voltage_ready_to_plot(means, IR_transmission, time):
    start_of_messages = get_IR_messages(IR_transmission, time)[2]
    number_of_messages = len(start_of_messages) - 1  # Since index starts from 0

    end_of_messages = []
    for i in range(len(start_of_messages)):
        end_of_messages.append(start_of_messages[i] + MSG_DURATION)

    avg_v = []
    msg_counter = 0

    i = 0
    while i < len(time):
        if((i < start_of_messages[msg_counter]) or (i > end_of_messages[number_of_messages])):
            avg_v.append(0)
        else:
            avg_v.append(means[msg_counter])
            while i < end_of_messages[msg_counter]:
                avg_v.append(means[msg_counter])
                i += 1

            if(msg_counter < number_of_messages):
                msg_counter += 1

        i += 1

    return avg_v


def get_msg_max_amplitude(IR_transmission, time):
    messages = get_IR_messages(IR_transmission, time)[0]
    
    max_values = []

    for msg in messages:
      max_values.append(max(msg))

    return max_values


if __name__ == "__main__":
    main()
