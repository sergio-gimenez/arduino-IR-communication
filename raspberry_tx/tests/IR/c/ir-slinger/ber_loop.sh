#!/bin/bash

for i in {0..5}
do
	./ber_tx
	python i2c_example.py
done

