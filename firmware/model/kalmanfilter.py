import numpy as np
import matplotlib.pyplot as plt
import math

from filterpy.kalman import UnscentedKalmanFilter
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import MerweScaledSigmaPoints

def load_dat_file(datfile):    
    press = []
    acc = []

    with open(datfile) as file:
        for line in file.readlines():
            values = line.split(' ')
            # Get pressure
            press.append(int(values[5]))

            # Get acceleration
            gx = float(values[6])
            gy = float(values[7])
            gz = float(values[8])
            ax = float(values[9])
            ay = float(values[10])
            az = float(values[11])
            
            # Calculate the vertical component of the acceleration
            absg = math.sqrt(gx*gx + gy*gy + gz*gz)
            skag = gx*ax + gy*ay + gz*az
            # measured offset of the sensor is 0.029512
            a = skag/absg - 0.029512
            acc.append(a)

    return press, acc

# Load sensor data
raw_press, raw_acc = load_dat_file('/home/zmolnar/vario_test.dat')

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

def res_fn(a, b):
    return (a-b)

# Create UKF
sigmas = MerweScaledSigmaPoints(n=3, alpha=.3, beta=2., kappa=.1)
ukf = UnscentedKalmanFilter(dim_x=3, dim_z=2, dt=.02, hx=hx, fx=fx, points=sigmas,residual_x=res_fn)

# Initial conditions
height = 44330 * (1 - pow ((raw_press[0] / 101325), 0.1902))
ukf.x = np.array([height, 0, 0])
print(f"Initial height: {height}")

# Process state covariance matrix
ukf.P *= np.array([[1,0,0],[0,1,0],[0,0,1]])

# Measurement covariance matrix
# Measured pressure std is 3.74
# Measured acc std is 0.02
ukf.R = np.array([[4**2, 0],[0, .02**2]])

# Process noise
ukf.Q = Q_discrete_white_noise(3, dt=.02, var=0.03)
print(ukf.Q)

zs, hs, vs, accs = [], [], [], []

for i in range(len(raw_press) - 1):
    meas = [raw_press[i+1], raw_acc[i+1]]
    ukf.predict()
    ukf.update(meas)
    hs.append(ukf.x[0])
    vs.append(ukf.x[1])
    accs.append(ukf.x[2])

fig, ax = plt.subplots(3,1) 
ax[0].plot(hs, label='height')
ax[1].plot(vs, label='speed')
ax[2].plot(accs, label='acceleration')

ax[0].legend()
ax[1].legend()
ax[2].legend()
plt.show()