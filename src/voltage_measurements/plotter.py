#!/usr/bin/env python3

import matplotlib.pyplot as plt
import sys

MSG_NOISE_THRESHOLD = 150 # In mV
MSG_DURATION = 100 # In milliseconds

def main():

    RECORDED_DATA_FILENAME = "out.csv"
    time = []
    v_rx_amp = []
    v_rx_preamp = []
    v_tx = []

    v_rx_amp, v_rx_preamp, v_tx, time = get_voltages_from_file(
        RECORDED_DATA_FILENAME)

    rx_messages, time_acc = get_IR_messages(v_rx_preamp, time)
    tx_messages, time_acce = (get_IR_messages(v_tx, time))

    #plt.plot(time_acc[0], rx_messages[0], label="Messages pre-amp", color='orange')
    #plt.plot(time_acc[1], rx_messages[1], label="Messages pre-amp", color='red')
    #format_plot('Time [s]', 'Voltage [mV]', " ")
  
    non_zeros_rx = remove_PWM_gaps(rx_messages)
    
    non_zeros_tx = remove_PWM_gaps(tx_messages)

    avg_rx_means = compute_avg(non_zeros_rx)
    avg_tx_means = compute_avg(non_zeros_tx)  
    

def get_voltages_from_file(output_filename):
    # Read the file and save it in an array without the \n character
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

    i = 0
    while i < (len(time)):

        # Remove noise and only get IR messages
        if (IR_transmission[i] > MSG_NOISE_THRESHOLD):
            start_of_msg = i

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
    
    return IR_messages, time_messages

def remove_PWM_gaps(IR_messages):

    no_zeros_msg = []
    no_zeros_messages = [] 

    for msg in IR_messages:
        for value in msg:
            if(value > MSG_NOISE_THRESHOLD):
                no_zeros_msg.append(value)
        
        no_zeros_messages.append(no_zeros_msg)
        #Delete values in no_zeros_msg since they are already stored in no_zeros_messages
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
    

def plot_means(means, IR_transmission, time):
    #get_IR_messages(IR_transmission, time)
    pass





if __name__ == "__main__":
    main()
