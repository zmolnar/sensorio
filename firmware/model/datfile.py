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
                    # Get pressure
                    self.p.append(int(values[self.pindex]))

                    # Get gravity vector
                    gx = float(values[self.gxindex])
                    gy = float(values[self.gyindex])
                    gz = float(values[self.gzindex])

                    g = math.sqrt(gx*gx + gy*gy + gz*gz)

                    # Calculate linear acceleration 
                    ax = float(values[self.axindex]) / 1000 * g
                    ay = float(values[self.ayindex]) / 1000 * g
                    az = float(values[self.azindex]) / 1000 * g

                    ax = self.x_scale * (ax - self.x_neg_1g) + self.ref_neg_1g
                    ay = self.y_scale * (ay - self.y_neg_1g) + self.ref_neg_1g
                    az = self.z_scale * (az - self.z_neg_1g) + self.ref_neg_1g

                    # Calculate the vertical component of the acceleration
                    absg = math.sqrt(gx*gx + gy*gy + gz*gz)
                    skag = gx*ax + gy*ay + gz*az
                    a = skag/absg - g

                    self.gx.append(gx)
                    self.gy.append(gy)
                    self.gz.append(gz)
                    self.ax.append(ax)
                    self.ay.append(ay)
                    self.az.append(az)
                    self.g.append(g)
                    self.a.append(a)

                    self.sampleCount += 1

    def __repr__(self):
        str = f'Sample count: {self.sampleCount}'
        return str

    def print(self):
        for i in range(self.sampleCount):
            str  = f'{self.g[i]} {self.gx[i]} {self.gy[i]} {self.gz[i]}'
            str += ' '
            str += f'{self.a[i]} {self.ax[i]} {self.ay[i]} {self.az[i]}'

            print(str)