import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
import math
import sys

def load_dat_file(datfile):    
    press = []
    acc = []
    axs = []
    ays = []
    azs = []
    gs  = []

    with open(datfile) as file:
        for line in file.readlines():
            if (3 < len(line)):
                values = line.split(' ')
                # Get pressure
                press.append(int(values[4]))

                # Get acceleration
                gx = float(values[5])
                gy = float(values[6])
                gz = float(values[7])

                g = math.sqrt(gx*gx +gy*gy + gz*gz)
                
                ax = float(values[8]) / 1000 * g
                ay = float(values[9]) / 1000 * g
                az = float(values[10]) / 1000 * g
                
                # # Calculate the vertical component of the acceleration
                # absg = math.sqrt(gx*gx + gy*gy + gz*gz)
                # skag = gx*ax + gy*ay + gz*az
                # # measured offset of the sensor is 0.029512
                # a = skag/absg - 9.8
                a = math.sqrt(ax*ax + ay*ay + az*az)
                gs.append(g)
                acc.append(a)
                axs.append(ax)
                ays.append(ay)
                azs.append(az)

    return press, acc, axs, ays, azs, gs

# Load sensor data
if (3 != len(sys.argv)):
    print(sys.argv)
    print('Wrong argument list')
    exit()

path = sys.argv[1]
ifile = path + sys.argv[2]
cppfile = path + 'out_' + sys.argv[2]

# Load sensor data
raw_press, raw_acc, raw_axs, raw_ays, raw_azs, raw_gs = load_dat_file(ifile)

press = np.array(raw_press)
acc = np.array(raw_acc)
axs = np.array(raw_axs)
ays = np.array(raw_ays)
azs = np.array(raw_azs)
gs  = np.array(raw_gs)

print(f'Acceleration mean: {np.mean(acc)} std: {stats.tstd(acc)}')
print(f'Pressure mean: {np.mean(press)} std: {stats.tstd(press)}')
print(f'ax mean: {np.mean(axs)} std: {stats.tstd(axs)}')
print(f'ay mean: {np.mean(ays)} std: {stats.tstd(ays)}')
print(f'az mean: {np.mean(azs)} std: {stats.tstd(azs)}')
print(f'g  mean: {np.mean(gs)} std: {stats.tstd(gs)}')

# fig, ax = plt.subplots(2,1) 
# ax[0].plot(raw_press, label='pressure')
# ax[1].plot(raw_acc, label='acceleration')
# ax[0].legend()
# ax[1].legend()
# plt.show()