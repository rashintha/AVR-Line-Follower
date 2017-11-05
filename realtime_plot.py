import serial
import numpy as np
import math
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import bluetooth
import os
import _thread

MAC = '98:D3:32:30:DE:FF'
port = 1

socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
socket.connect((MAC, port))

print("Connected")

socket.settimeout(100.0)
plotx = [0]
ploty = [0]
plotval = []
count = 0
countx = 0

def beep(freq, duration):
    os.system('(speaker-test -t sine -f %f) & pid=$!; sleep %ss; kill -9 $pid' % (freq, duration))

while True:
    x = socket.recv(1)
    y = socket.recv(1)
    val = ord(x) + (ord(y) << 8)

    print(val)

    if val == 5555:
        #red
        _thread.start_new_thread(beep, (600, 0.5))
    elif val == 6666:
        #green
        count = count - 1
        ploty.append(plotval[int(count/4)])
        ploty.append(plotval[int(count/2)])
        ploty.append(plotval[int(count*3/4)])
        ploty.append(plotval[count])

        plotx.append(countx + 1/4)
        plotx.append(countx + 1/2)
        plotx.append(countx + 3/4)
        plotx.append(countx + 1)
        countx = countx + 1
        plotval = []
        count = 0
    else:
        plotval.append(val)
        count = count + 1

#    if (1 & ord(x)):
#        valList.append(1)

#    if (2 & ord(x)):
#        valList.append(2)

    print(ord(x))

    plt.plot(plotx, ploty, 'b--')
    plt.axis([0, countx, 0, 200])
    plt.pause(0.005)

    #plotList = plotList + valList
    #print(plotList)
    #print(valList)

    # calculating mean
    #mu = sum(plotList) / len(plotList)

    # calculating standard deviation
    #tempList = [math.pow(element - mu, 2) for element in plotList]
    #sigma = math.sqrt(sum(tempList) / (len(plotList) - 1))

    #print(mu)
    #print(sigma)

    #num_bins = 30

    # the histogram of the data
    #plt.clf()
    #n, bins, patches = plt.hist(plotList, num_bins, normed=1, facecolor='blue', alpha=0.5)

    # add a 'best fit' line
    #y = mlab.normpdf(bins, mu, sigma)
    #plt.plot(bins, y*4, 'r--')
    #plt.xlabel('IR')
    #plt.ylabel('Count')
    #axes = plt.gca()
    #axes.set_xlim([0,9])
    #plt.pause(0.005)