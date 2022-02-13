import matplotlib.pyplot as plt
import math
import sys
import os
import time
import numpy as np
from scipy import stats

from datfile import datfile
from ukf_withacc import ukf_with_acc
from ukf_noacc import ukf_no_acc

if (2 != len(sys.argv)):
    print(sys.argv)
    print('Usage')
    print('>> python compare_solutions.py path-to-datfile')
    exit()

input = datfile(sys.argv[1])
input.load()

# Setup model parameters
dt = .02
p_std = stats.tstd(input.p)
a_std = stats.tstd(input.a)

# Print statistics
print('1. Statistics')
print(f'  Pressure')
print(f'    Count : {len(input.p)}')
print(f'    Mean  : {np.mean(input.p)}')
print(f'    Std   : {stats.tstd(input.p)}')
print(f'  Acceleration')
print(f'    Count : {len(input.a)}')
print(f'    Mean  : {np.mean(input.a)}')
print(f'    Std   : {stats.tstd(input.a)}')

# Setup and run UKF with acceleration
print('2. Running UKF with acceleration ...', end = ' ', flush = True),
ukf_wacc = ukf_with_acc(dt)
ukf_wacc.init(input.p[0], input.a[0], p_std, a_std)
start = time.time()
h_wacc, v_wacc, a_wacc = ukf_wacc.run(input.p, input.a);
end = time.time()
print(f'{end - start} seconds')

# Setup and run UKF without acceleration, std is 3.0
print('3. Running UKF without acceleration, std is 3.0 ...', end = ' ', flush = True),
ukf_noacc = ukf_no_acc(dt)
ukf_noacc.init(input.p[0], 3.0)
start = time.time()
h_noacc_1, v_noacc_1 = ukf_noacc.run(input.p)
end = time.time()
print(f'{end - start} seconds')

# Setup and run UKF without acceleration, std is 1.5
print('3. Running UKF without acceleration, std is 1.5 ...', end = ' ', flush = True),
ukf_noacc = ukf_no_acc(dt)
ukf_noacc.init(input.p[0], 1.5)
start = time.time()
h_noacc_2, v_noacc_2 = ukf_noacc.run(input.p)
end = time.time()
print(f'{end - start} seconds')

# Setup and run UKF without acceleration, std is 0.5
print('3. Running UKF without acceleration, std is 0.5 ...', end = ' ', flush = True),
ukf_noacc = ukf_no_acc(dt)
ukf_noacc.init(input.p[0], 0.5)
start = time.time()
h_noacc_3, v_noacc_3 = ukf_noacc.run(input.p)
end = time.time()
print(f'{end - start} seconds')

# Difference in vario
v_diff = []
for i in range(len(v_wacc)):
    v_diff.append(v_wacc[i] - v_noacc_1[i])

# Visualize results
fig, ax = plt.subplots(3,1)
fig.suptitle('Sensorio')
ax[0].set_title('Height')
ax[0].plot(h_wacc, label='With acceleration')
ax[0].plot(h_noacc_1, label='No acceleration, std=3.0')
ax[0].plot(h_noacc_2, label='No acceleration, std=1.5')
ax[0].plot(h_noacc_3, label='No acceleration, std=0.5')
ax[0].legend()

ax[1].set_title('Vario')
ax[1].plot(v_wacc, label='With acceleration')
ax[1].plot(v_noacc_1, label='No acceleration, std=3.0')
ax[1].plot(v_noacc_2, label='No acceleration, std=1.5')
ax[1].plot(v_noacc_3, label='No acceleration, std=0.5')
ax[1].legend()

ax[2].set_title('Difference in vario')
ax[2].plot(v_diff, label='Diff')
ax[2].legend()

fig2, ax2 = plt.subplots(2,1)
fig2.suptitle('Raw data')
ax2[0].plot(input.p, label='pressure')
ax2[1].plot(input.a, label='acceleration')
ax2[0].legend()
ax2[1].legend()

plt.show()
