#!/bin/bash

OUTPUT_FILE="out.csv" 
SERIAL_PORT="/dev/ttyACM0"
TIMEOUT=7
RASPBERRY_IP=192.168.1.10

echo " "
echo "*************************************************************"
echo "* Make sure arduino has been reset before the transmission! *"
echo "*************************************************************"
echo " "

rm $OUTPUT_FILE 2> /dev/null

# Read the serial port and print the msgs on a file in another thread
(tail -f $SERIAL_PORT > $OUTPUT_FILE) &
disown # Make the kill command quiet

# Open ssh connection to the raspberry and send IR data
ssh -t pi@$RASPBERRY_IP -i ~/.ssh/sergio-laptop.key "sudo /home/pi/tfg/src/raspberry_tx/ir-slinger/send_ones.sh"

# Read all the messages sent from the raspberry
# Make sure that the timeout is enough to receive all the IR msgs.
echo "Waiting ($TIMEOUT seconds) for transmission to end..."
SECONDS=0
while [ $SECONDS -le $TIMEOUT ]
do :
done

kill -9 $(pidof tail)

# Plot the results
./voltages_plot.py



