#!/bin/sh

pigpiod
./pcs -d
./pcs -tar 0 0 255
./pcs -st green 0
