import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
import math
import sys
import os

from datfile import datfile
from filter import Filter

from filterpy.kalman import UnscentedKalmanFilter
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import MerweScaledSigmaPoints

# State transition function
def fx(x, dt):
    xout = np.empty_like(x)
    xout[0] = (x[2] * dt * dt / 2) + (x[1] * dt) + x[0]
    xout[1] = x[2] * dt + x[1]
    xout[2] = x[2]

    return xout

# Measurement function
def hx(x):
    h = 101325 * pow((1 - 2.25577 * pow(10, -5) * x[0]), 5.25588)
    a = x[2]

    return np.array([h, a])

# Load sensor data
if (2 != len(sys.argv)):
    print(sys.argv)
    print('Wrong argument list')
    exit()

input = datfile(sys.argv[1])
input.pindex = 0
input.gxindex = 1
input.gyindex = 2
input.gzindex = 3
input.axindex = 4
input.ayindex = 5
input.azindex = 6

input.load()
raw_p, raw_a = input.process()

p_std1 = 3.74
a_std1 = .4 
filter1 = Filter(fx, hx)
filter1.initialize(raw_p[0], raw_a[0], p_std1, a_std1)
heights1, varios1, accs1 = filter1.run(raw_p, raw_a)

p_std2 = 3.74
a_std2 = .8 
filter2 = Filter(fx, hx)
filter2.initialize(raw_p[0], raw_a[0], p_std2, a_std2)
heights2, varios2, accs2 = filter2.run(raw_p, raw_a)

p_std3 = 3.74
a_std3 = 1.2 
filter3 = Filter(fx, hx)
filter3.initialize(raw_p[0], raw_a[0], p_std3, a_std3)
heights3, varios3, accs3 = filter3.run(raw_p, raw_a)

fig, ax = plt.subplots(3,1) 
fig.suptitle('Filterpy')
ax[0].set_title('Height')
ax[0].plot(heights1, label=f'std = {filter1.a_std}')
ax[0].plot(heights2, label=f'std = {filter2.a_std}')
ax[0].plot(heights3, label=f'std = {filter3.a_std}')
ax[0].legend()

ax[1].set_title('Vario')
ax[1].plot(varios1, label=f'std = {filter1.a_std}')
ax[1].plot(varios2, label=f'std = {filter2.a_std}')
ax[1].plot(varios3, label=f'std = {filter3.a_std}')
ax[1].legend()

ax[2].set_title('Acceleration')
ax[2].plot(accs1, label=f'std = {filter1.a_std}')
ax[2].plot(accs2, label=f'std = {filter2.a_std}')
ax[2].plot(accs3, label=f'std = {filter3.a_std}')
ax[2].plot(raw_a, label='raw')
ax[2].legend()

fig2, ax2 = plt.subplots(2,1)
fig2.suptitle('Raw data')
ax2[0].plot(raw_p, label='pressure')
ax2[1].plot(raw_a, label='acceleration')
ax2[0].legend()
ax2[1].legend()

plt.show()