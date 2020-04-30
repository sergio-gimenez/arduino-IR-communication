#!/bin/bash

OUTPUT_FILE="out.csv"
SERIAL_PORT="/dev/ttyACM0"

echo "Make sure arduino has been reset before!"
rm $OUTPUT_FILE

(tail -f /dev/ttyACM0 > out.csv) &

SECONDS=0
while [ $SECONDS -le 8 ]
do
  echo "Waiting for transmission to end"
done

kill -9 $(pidof tail)

./plotter.py




