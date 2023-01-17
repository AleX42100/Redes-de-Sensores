# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.

import serial   #allows to use the serial port functions

port_serie = serial.Serial(port = '/dev/cu.SLAB_USBtoUART', baudrate = 115200)  # same parameters from the Arduino code

while 1:
    s = port_serie.readline().decode()
    print(s)
    file = open("DataSensor.txt", "a")
    file.writelines(s)
