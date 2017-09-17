import serial
import numpy as np
import math
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

plotList = [1, 2, 3, 4, 5, 6, 7, 8]

while True:
    with serial.Serial("/dev/ttyUSB0", 9600, timeout=1) as ser:
        x = ser.read()

        valList = []

        if not (1 & ord(x)):
            valList.append(1)

        if not (2 & ord(x)):
            valList.append(2)

        if not (4 & ord(x)):
            valList.append(3)

        if not (8 & ord(x)):
            valList.append(4)

        if not (16 & ord(x)):
            valList.append(5)

        if not (32 & ord(x)):
            valList.append(6)

        if not (64 & ord(x)):
            valList.append(7)

        if not (128 & ord(x)):
            valList.append(8)

        plotList = plotList + valList
        print(plotList)

        # calculating mean
        mu = sum(plotList) / len(plotList)

        # calculating standard deviation
        tempList = [math.pow(element - mu, 2) for element in plotList]
        sigma = math.sqrt(sum(tempList) / (len(plotList) - 1))

        print(mu)
        print(sigma)

        num_bins = 30

        # the histogram of the data
        plt.clf()
        n, bins, patches = plt.hist(plotList, num_bins, normed=1, facecolor='blue', alpha=0.5)

        # add a 'best fit' line
        y = mlab.normpdf(bins, mu, sigma)
        plt.plot(bins, y*4, 'r--')
        plt.xlabel('IR')
        plt.ylabel('Count')
        axes = plt.gca()
        axes.set_xlim([0,9])
        plt.pause(0.005)