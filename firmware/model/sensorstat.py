import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
import math

def load_dat_file(datfile):    
    press = []
    acc = []

    with open(datfile) as file:
        for line in file.readlines():
            values = line.split(' ')
            press.append(int(values[5]))
            gx = float(values[6])
            gy = float(values[7])
            gz = float(values[8])
            ax = float(values[9])
            ay = float(values[10])
            az = float(values[11])
            
            absg = math.sqrt(gx*gx + gy*gy + gz*gz)
            skag = gx*ax + gy*ay + gz*az
            a = skag/absg
            acc.append(a)

    return press, acc

# Load sensor data
raw_press, raw_acc = load_dat_file('/home/zmolnar/vario_standstill.dat')

press = np.array(raw_press)
acc = np.array(raw_acc)

print(f'Acceleration mean: {np.mean(acc)} std: {stats.tstd(acc)}')
print(f'Pressure mean: {np.mean(press)} std: {stats.tstd(press)}')

fig, ax = plt.subplots(2,1) 
ax[0].plot(raw_press, label='pressure')
ax[1].plot(raw_acc, label='acceleration')
ax[0].legend()
ax[1].legend()
plt.show()