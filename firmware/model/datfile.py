import math

class datfile:
    def __init__(self, ifile):
        self.ifile = ifile
        self.sampleCount = 0

        self.gx = []
        self.gy = []
        self.gz = []
        self.ax = []
        self.ay = []
        self.az = []
        self.p = []
        self.g = []
        self.a = []

        self.gxindex = 0
        self.gyindex = 0
        self.gzindex = 0
        self.axindex = 0
        self.ayindex = 0
        self.azindex = 0
        self.pindex = 0

        # Calibration points
        self.x_pos_1g = 9.7634
        self.x_neg_1g = -9.5352
        self.y_pos_1g = 9.5581
        self.y_neg_1g = -9.7759
        self.z_pos_1g = 9.8541
        self.z_neg_1g = -9.5001

        # Calibration reference
        self.ref_pos_1g = 9.8
        self.ref_neg_1g = -9.8

        #
        # Calculated scaling factor based on calibration
        #
        self.x_scale = (self.ref_pos_1g - self.ref_neg_1g) / (self.x_pos_1g - self.x_neg_1g)
        self.y_scale = (self.ref_pos_1g - self.ref_neg_1g) / (self.y_pos_1g - self.y_neg_1g)
        self.z_scale = (self.ref_pos_1g - self.ref_neg_1g) / (self.z_pos_1g - self.z_neg_1g)

    def load(self):
        with open(self.ifile) as file:
            for line in file.readlines():
                if (3 < len(line)):
                    values = line.split(' ')
                    self.p.append(int(values[self.pindex]))
                    self.gx.append(float(values[self.gxindex]))
                    self.gy.append(float(values[self.gyindex]))
                    self.gz.append(float(values[self.gzindex]))
                    self.ax.append(float(values[self.axindex]))
                    self.ay.append(float(values[self.ayindex]))
                    self.az.append(float(values[self.azindex]))

                    self.sampleCount += 1

    def process(self):
        raw_p, raw_a = [], []

        for i in range(self.sampleCount):
            gx = self.gx[i]
            gy = self.gy[i]
            gz = self.gz[i]

            absg = math.sqrt(gx*gx + gy*gy + gz*gz)

            ax = self.ax[i] / 1000 * absg
            ay = self.ay[i] / 1000 * absg
            az = self.az[i] / 1000 * absg

            ax = self.x_scale * (ax - self.x_neg_1g) + self.ref_neg_1g
            ay = self.y_scale * (ay - self.y_neg_1g) + self.ref_neg_1g
            az = self.z_scale * (az - self.z_neg_1g) + self.ref_neg_1g

            # Calculate the vertical component of the acceleration
            skag = gx*ax + gy*ay + gz*az
            a = skag/absg - absg

            raw_p.append(self.p[i])
            raw_a.append(a)

        return raw_p, raw_a

    def __repr__(self):
        str = f'Sample count: {self.sampleCount}'
        return str

    def print(self):
        for i in range(self.sampleCount):
            str  = f'{self.gx[i]} {self.gy[i]} {self.gz[i]}'
            str += ' '
            str += f'{self.ax[i]} {self.ay[i]} {self.az[i]}'

            print(str)