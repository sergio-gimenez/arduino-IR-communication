#!/bin/bash

for i in {0..1000}
do
	./generate_rand
	python i2c_example.py
	./send_IR_signal
done

