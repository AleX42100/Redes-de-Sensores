# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.

import serial   #allows to use the serial port functions
import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

port_serie = serial.Serial(port = '/dev/cu.SLAB_USBtoUART', baudrate = 115200)  # same parameters from the Arduino code


id=0    #identifier of the datapoint


while 1:
    i=0
    fiveseclist = []
    while i<5:
        SERIALDATA = port_serie.readline().split()  # Read line of text from Arduino
        #print(SERIALDATA) #debug
        fiveseclist.extend(SERIALDATA)
        #print(fiveseclist) #debug
        i=i+1
    for i in range (0, len(fiveseclist)):
        fiveseclist[i]=float(fiveseclist[i])
    print(fiveseclist)
    avg=numpy.average(fiveseclist)
    std=numpy.std(fiveseclist)
    print("Average: ",avg) #Moyenne
    print("Standard Deviation: ",std) #Écart-type
    plt.scatter(id,avg)
    plt.scatter(id,std)
    plt.show()
    id = id + 1