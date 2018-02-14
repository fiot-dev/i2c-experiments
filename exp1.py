import smbus
bus = smbus.SMBus(1)
address = 0x2a
while (1):
    block = bus.read_i2c_block_data(address, 0, 2) # Returned value is a list of 2 bytes
    print(block)
    delay(100)
