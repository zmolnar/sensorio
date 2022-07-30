import numpy as np

from filterpy.kalman import UnscentedKalmanFilter
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import MerweScaledSigmaPoints


class ukf_with_acc:
    def __init__(self, dt):
        self.dt = dt
        self.sigmas = MerweScaledSigmaPoints(
            n=3, alpha=.3, beta=2., kappa=.1)
        self.ukf = UnscentedKalmanFilter(
            dim_x=3, dim_z=2, dt=dt, hx=self.hx, fx=self.fx, points=self.sigmas)

    def fx(self, x, dt):
        xout = np.empty_like(x)
        xout[0] = (x[2] * dt * dt / 2) + (x[1] * dt) + x[0]
        xout[1] = x[2] * dt + x[1]
        xout[2] = x[2]
        return xout

    def hx(self, x):
        p = 101325 * pow((1 - 2.25577 * pow(10, -5) * x[0]), 5.25588)
        a = x[2]
        return np.array([p, a])

    def init(self, p0, a0, p_std, a_std):
        height = 44330 * (1 - pow ((p0 / 101325), 0.1902))
        self.ukf.x = np.array([height, 0, a0])
        self.ukf.P *= np.array([[1,0,0],[0,1,0],[0,0,1]])
        self.ukf.R = np.array([[p_std**2, 0],[0, a_std**2]])
        self.ukf.Q = Q_discrete_white_noise(3, dt=self.dt, var=.03)

    def run(self, raw_p, raw_a):
        heights, varios, accs = [], [], []

        for i in range(len(raw_p)):
            meas = [raw_p[i], raw_a[i]]
            self.ukf.predict()
            self.ukf.update(meas)

            heights.append(self.ukf.x[0])
            varios.append(self.ukf.x[1])
            accs.append(self.ukf.x[2])

        return heights, varios, accs
