import matplotlib.pyplot as plt
import csv

time = []
voltage = []

# Read the file and save it in an array without the \n character
# Each position has: [V,t]
out = open("out.csv", "r").read().splitlines()
for i in out:
    voltage.append(float(i.split(",")[0]))
    time.append(int(i.split(",")[1]))


#import pdb; pdb.set_trace()

plt.plot(time,voltage)
plt.xlabel('Time [s]')
plt.ylabel('Voltage [mV]')
plt.title('Received Voltage\nReceived 6 messages (32bit each)')
plt.xlim(0,3000)
plt.grid()
plt.show()

