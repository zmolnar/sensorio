import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
import math
import sys
import os

from filterpy.kalman import UnscentedKalmanFilter
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import MerweScaledSigmaPoints

x_pos_1g = 9.7634
x_neg_1g = -9.5352
y_pos_1g = 9.5581
y_neg_1g = -9.7759
z_pos_1g = 9.8541
z_neg_1g = -9.5001

ref_pos_1g = 9.8
ref_neg_1g = -9.8

x_scale = (ref_pos_1g - ref_neg_1g) / (x_pos_1g - x_neg_1g)
y_scale = (ref_pos_1g - ref_neg_1g) / (y_pos_1g - y_neg_1g)
z_scale = (ref_pos_1g - ref_neg_1g) / (z_pos_1g - z_neg_1g)

def load_dat_file(datfile):    
    press = []
    acc = []

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

                g = math.sqrt(gx*gx + gy*gy + gz*gz)

                ax = float(values[8]) / 1000 * g
                ay = float(values[9]) / 1000 * g
                az = float(values[10]) / 1000 * g

                ax = x_scale * (ax - x_neg_1g) + ref_neg_1g
                ay = y_scale * (ay - y_neg_1g) + ref_neg_1g
                az = z_scale * (az - z_neg_1g) + ref_neg_1g


                # # Calculate the vertical component of the acceleration
                absg = math.sqrt(gx*gx + gy*gy + gz*gz)
                skag = gx*ax + gy*ay + gz*az
                # # measured offset of the sensor is 0.029512
                a = skag/absg - g

                print(g, gx, gy, gz, ax, ay, az, a)

                acc.append(a)

    return press, acc


def load_output_file(outfile):    
    height = []
    vario = []
    acc = []

    with open(outfile) as file:
        for line in file.readlines():
            values = line.split(' ')
            height.append(float(values[0]))
            vario.append(float(values[1]))
            acc.append(float(values[2]))

    return height, vario, acc

# Load sensor data
if (3 != len(sys.argv)):
    print(sys.argv)
    print('Wrong argument list')
    exit()

path = sys.argv[1]
ifile = path + sys.argv[2]
cppfile = path + 'out_' + sys.argv[2]

print(ifile)
print(cppfile)

raw_press, raw_acc = load_dat_file(ifile)

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
ukf.R = np.array([[4**2, 0],[0, .4**2]])

# Process noise
ukf.Q = Q_discrete_white_noise(3, dt=.02, var=0.03)
print(ukf.Q)

zs, hs, vs, accs = [], [], [], []

#
# Run FilterPy UKF implementation as a reference
#
for i in range(len(raw_press) - 1):
    meas = [raw_press[i+1], raw_acc[i+1]]
    ukf.predict()
    ukf.update(meas)
    hs.append(ukf.x[0])
    vs.append(ukf.x[1])
    accs.append(ukf.x[2])

fig, ax = plt.subplots(4,1) 
fig.suptitle('Filterpy')
ax[0].plot(hs, label='height')
ax[1].plot(vs, label='speed')
ax[2].plot(accs, label='acceleration')
ax[0].legend()
ax[1].legend()
ax[2].legend()


print(f'Acceleration mean: {np.mean(raw_acc)} std: {stats.tstd(raw_acc)}')

'''
#
# Run Kalman filter C++ implementation
#
simulator  = "build/src/sensorio-cppmodel_run" + " "
simulator += sys.argv[1] + " "
simulator += sys.argv[2] 
os.system(simulator)

#
# Read the output file
#
cppheight, cppvario, cppacc = load_output_file(cppfile)
#
# Plot simulation output
#
fig2, ax2 = plt.subplots(3,1)
fig2.suptitle('C++ implementation')
ax2[0].plot(cppheight, label='height')
ax2[1].plot(cppvario, label='speed')
ax2[2].plot(cppacc, label='acc')


ax2[0].legend()
ax2[1].legend()
ax2[2].legend()
'''

plt.show()