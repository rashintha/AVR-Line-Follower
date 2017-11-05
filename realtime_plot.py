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

    print(ord(x))

    plt.plot(plotx, ploty, 'b--')
    plt.axis([0, countx, 0, 200])
    plt.pause(0.005)