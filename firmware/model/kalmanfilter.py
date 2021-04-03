import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
import math
import sys
import os

from datfile import datfile

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
input.pindex = 4
input.gxindex = 5
input.gyindex = 6
input.gzindex = 7
input.axindex = 8
input.ayindex = 9
input.azindex = 10

input.load()

print(input)
input.print()

#
# Create UKF
#
sigmas = MerweScaledSigmaPoints(n=3, alpha=.3, beta=2., kappa=.1)
ukf = UnscentedKalmanFilter(dim_x=3, dim_z=2, dt=.02, hx=hx, fx=fx, points=sigmas)

# Initial conditions
height = 44330 * (1 - pow ((input.p[0] / 101325), 0.1902))
ukf.x = np.array([height, 0, input.a[0]])

# Process state covariance matrix
ukf.P *= np.array([[1,0,0],[0,1,0],[0,0,1]])

# Measurement covariance matrix
# Measured pressure std is 3.74
# Measured acc std is 0.02
p_std = 3.74
a_std = .04
ukf.R = np.array([[p_std**2, 0],[0, a_std**2]])

# Process noise
ukf.Q = Q_discrete_white_noise(3, dt=.02, var=0.03)

heights, varios, accs = [], [], []

#
# Run FilterPy UKF implementation as a reference
#
for i in range(input.sampleCount - 1):
    meas = [input.p[i+1], input.a[i+1]]
    ukf.predict()
    ukf.update(meas)

    heights.append(ukf.x[0])
    varios.append(ukf.x[1])
    accs.append(ukf.x[2])

fig, ax = plt.subplots(3,1) 
fig.suptitle('Filterpy')

ax[0].plot(heights, label='height')
ax[1].plot(varios, label='speed')
ax[2].plot(accs, label='acceleration')

ax[0].legend()
ax[1].legend()
ax[2].legend()

plt.show()