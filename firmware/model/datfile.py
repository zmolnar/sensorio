import sys
import math

class datfile:
    def __init__(self, ifile):
        self.ifile = ifile
        self.sampleCount = 0
        self.length = 0

        self.ts = []
        self.dt = []
        self.p  = []
        self.gx = []
        self.gy = []
        self.gz = []
        self.ax = []
        self.ay = []
        self.az = []
        self.h  = []
        self.v  = []
        self.a  = []

        self.tsindex = 0
        self.dtindex = 0
        self.pindex  = 0
        self.gxindex = 0
        self.gyindex = 0
        self.gzindex = 0
        self.axindex = 0
        self.ayindex = 0
        self.azindex = 0
        self.hindex  = 0
        self.vindex  = 0
        self.aindex  = 0


    def setup(self, header):
        tags = header.rstrip('\n').split(' ')
        self.length = len(tags)
        for i in range(self.length):
            tag = tags[i]
            if tag == 'ts':
                self.tsindex = i
            elif tag == 'dt':
                self.dtindex = i
            elif tag == 'p':
                self.pindex = i
            elif tag == 'gx':
                self.gxindex = i
            elif tag == 'gy':
                self.gyindex = i
            elif tag == 'gz':
                self.gzindex = i
            elif tag == 'ax':
                self.axindex = i
            elif tag == 'ay':
                self.ayindex = i
            elif tag == 'az':
                self.azindex = i
            elif tag == 'h':
                self.hindex = i
            elif tag == 'v':
                self.vindex = i
            elif tag == 'a':
                self.aindex = i
            else:
                sys.exit('invalid tag in datfile: {}'.format(tag))

    def load(self):
        with open(self.ifile) as file:
            header = file.readline();
            self.setup(header)
            for line in file.readlines():
                values = line.split(' ')
                if(self.length == len(values)):
                    for i in range(self.length):
                        value = values[i]
                        if i == self.tsindex:
                            self.ts.append(int(value))
                        elif i == self.dtindex:
                            self.dt.append(float(value))
                        elif i == self.pindex:
                            self.p.append(int(value))
                        elif i == self.gxindex:
                            self.gx.append(float(value))
                        elif i == self.gyindex:
                            self.gy.append(float(value))
                        elif i == self.gzindex:
                            self.gz.append(float(value))
                        elif i == self.axindex:
                            self.ax.append(float(value))
                        elif i == self.ayindex:
                            self.ay.append(float(value))
                        elif i == self.azindex:
                            self.az.append(float(value))
                        elif i == self.hindex:
                            self.h.append(float(value))
                        elif i == self.vindex:
                            self.v.append(float(value))
                        elif i == self.aindex:
                            self.a.append(float(value))
                        else:
                            sys.exit('invalid line in datfile: {line}')

                    self.sampleCount += 1


    def print(self):
        print('ts index: {}'.format(self.tsindex))
        print('dt index: {}'.format(self.dtindex))
        print('p  index: {}'.format(self.pindex))
        print('gx index: {}'.format(self.gxindex))
        print('gy index: {}'.format(self.gyindex))
        print('gz index: {}'.format(self.gzindex))
        print('ax index: {}'.format(self.axindex))
        print('ay index: {}'.format(self.ayindex))
        print('az index: {}'.format(self.azindex))
        print('h index: {}'.format(self.hindex))
        print('v index: {}'.format(self.vindex))
        print('a index: {}'.format(self.aindex))
