#!/bin/bash

for i in {0..5000}
do
	./generate_rand
	python i2c_com.py
	./send_IR_signal
done

