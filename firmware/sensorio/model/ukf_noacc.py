import numpy as np

from filterpy.kalman import UnscentedKalmanFilter
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import MerweScaledSigmaPoints


class ukf_no_acc:
    def __init__(self, dt):
        self.sigmas = MerweScaledSigmaPoints(
            n=2, alpha=.3, beta=2., kappa=.1)
        self.ukf = UnscentedKalmanFilter(
            dim_x=2, dim_z=1, dt=dt, hx=self.hx, fx=self.fx, points=self.sigmas)

    def fx(self, x, dt):
        xout = np.empty_like(x)
        xout[0] = (x[1] * dt) + x[0]
        xout[1] = x[1]
        return xout

    def hx(self,x):
        h = 101325 * pow((1 - 2.25577 * pow(10, -5) * x[0]), 5.25588)
        return np.array([h])

    def init(self, p0, p_std):
        height = 44330 * (1 - pow ((p0 / 101325), 0.1902))
        self.ukf.x = np.array([height, 0])
        self.ukf.P *= np.array([[1,0],[0,1]])
        self.ukf.R = p_std**2
        self.ukf.Q = Q_discrete_white_noise(2, dt=.02, var=0.03)

    def run(self, raw_p):
        heights, varios = [], []

        for i in range(len(raw_p)):
            self.ukf.predict()
            self.ukf.update(raw_p[i])

            heights.append(self.ukf.x[0])
            varios.append(self.ukf.x[1])

        return heights, varios
