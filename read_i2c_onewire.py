import smbus
import time
import struct

bus = smbus.SMBus(1)
address = 0x04

while True:
    try:
        data = bus.read_i2c_block_data(address, 0);
    except:
        print "Error getting data\n"
        continue
    # Example uses 4 sensors. 
    # Each float from the Arduino is 4 bytes long.
    for i in range(0, 4):
        bytes = data[4*i:4*i+4]
        # Python 2 struct.unpack takes the data to be unpacked 
        # in string format, so the bytes need to be joined
        # together first
        print struct.unpack('f', "".join(map(chr, bytes)))[0]
    print
    time.sleep(5);
