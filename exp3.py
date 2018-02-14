#!/usr/bin/env python
# coding: utf8

import smbus
import struct
import time
import requests
# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

# Set the request parameters

PROGRAM_NAME_VERSION = "I2C Reader v4.6.9"
DEBUG = True

def get_data():
    return bus.read_i2c_block_data(address, 0);

def get_float(data, index):
    bytes = data[4*index:(index+1)*4]
    return struct.unpack('f', "".join(map(chr, bytes)))[0]

## Begin ##
while True:
    data = get_data()
  
    dataWind = get_float(data, 0)*3.6

    time.sleep(1)
