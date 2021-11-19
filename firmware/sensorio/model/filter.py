
import numpy as np

from filterpy.kalman import UnscentedKalmanFilter
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import MerweScaledSigmaPoints


class Filter:
    def __init__(self, fx, hx):
        self.sigmas = MerweScaledSigmaPoints(
            n=3, alpha=.3, beta=2., kappa=.1)
        self.ukf = UnscentedKalmanFilter(
            dim_x=3, dim_z=2, dt=.02, hx=hx, fx=fx, points=self.sigmas)

    def initialize(self, p0, a0, p_std, a_std):
        self.p_std = p_std
        self.a_std = a_std
        height = 44330 * (1 - pow ((p0 / 101325), 0.1902))
        self.ukf.x = np.array([height, 0, a0])
        self.ukf.P *= np.array([[1,0,0],[0,1,0],[0,0,1]])
        self.ukf.R = np.array([[p_std**2, 0],[0, a_std**2]])
        self.ukf.Q = Q_discrete_white_noise(3, dt=.02, var=0.03)

    def run(self, raw_p, raw_a):
        heights, varios, accs = [], [], []

        for i in range(len(raw_p) - 1):
            meas = [raw_p[i+1], raw_a[i+1]]
            self.ukf.predict()
            self.ukf.update(meas)

            heights.append(self.ukf.x[0])
            varios.append(self.ukf.x[1])
            accs.append(self.ukf.x[2])

        return heights, varios, accs