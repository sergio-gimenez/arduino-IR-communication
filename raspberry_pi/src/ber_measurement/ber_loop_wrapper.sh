#!/bin/bash

TOTAL_MSGS=50000

TEMPFILE=/tmp/$$.tmp
TEMP_RES=/tmp/$$_res.tmp

echo 0 > $TEMPFILE
echo 0 > $TEMP_RES

for (( i = 1; i <= $TOTAL_MSGS; i++ )) 
do

	./generate_rand
	python i2c_com.py
	./send_IR_signal

	#Progress bar below
	COUNTER=$[$(cat $TEMPFILE) + 1]
	echo $COUNTER > $TEMPFILE
	echo "scale=2; ($(cat $TEMPFILE)/$TOTAL_MSGS)*100" | bc > $TEMP_RES
	echo "Progress: $(cat $TEMP_RES) %"

done

unlink $TEMPFILE
unlink $TEMP_RES
