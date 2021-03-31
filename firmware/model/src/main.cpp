/**
 * @file main.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <iostream>

#include "MerweScaledSigmaPoints.h"
#include "Ukf.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static const size_t dim_x = 3;
static const size_t dim_z = 2;
static const double dt    = 0.02;
static const double alpha = 0.3;
static const double beta  = 2.0;
static const double kappa = 0.1;

static const size_t buflength = 5000;

static size_t sampleCount;
static double z_press[buflength];
static double z_acc[buflength];
static double out_height[buflength];
static double out_vario[buflength];
static double out_acc[buflength];

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void readDatFile(const std::string &fileName)
{
  std::ifstream datfile(fileName);

  if (datfile.is_open()) {
    std::string line;
    while (std::getline(datfile, line)) {
      assert(sampleCount < buflength);

      std::stringstream        str(line);
      std::vector<std::string> numbers;
      std::string              number;
      while (getline(str, number, ' ')) {
        numbers.push_back(number);
      }

      assert(11 == numbers.size());

      double p  = ::atof(numbers.at(4).c_str());
      double gx = ::atof(numbers.at(5).c_str());
      double gy = ::atof(numbers.at(6).c_str());
      double gz = ::atof(numbers.at(7).c_str());
      double ax = ::atof(numbers.at(8).c_str());
      double ay = ::atof(numbers.at(9).c_str());
      double az = ::atof(numbers.at(10).c_str());

      // Calculate vertical component of the acceleration
      double absg = sqrt(gx * gx + gy * gy + gz * gz);
      double skag = gx * ax + gy * ay + gz * az;

      static const double a_offset = 0.029512;

      double a = skag / absg - a_offset;

      // Save values
      z_press[sampleCount] = p;
      z_acc[sampleCount]   = a;
      ++sampleCount;
    }
  }
}

// State transition function
static Matrix fx(const Vector &x, double dt)
{
  Matrix xout = Matrix(dim_x, 1);

  // Position     = a*dt2/2 + v*dt + x0
  // Speed        = a*dt + v
  // Acceleration = a

  xout(0) = (x(2) * dt * dt / 2) + (x(1) * dt) + x(0);
  xout(1) = (x(2) * dt) + x(1);
  xout(2) = x(2);

  return xout;
}

// Measurement function
static Matrix hx(const Vector &x)
{
  assert(dim_x == x.length);

  // Convert height to pressure at altitude using barometric formula
  double press = 101325 * pow((1 - 2.25577 * pow(10, -5) * x(0)), 5.25588);
  double acc   = x(2);

  Matrix zout = Matrix(2, 1);
  zout(0)     = press;
  zout(1)     = acc;

  return zout;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
int main(int argc, char *argv[])
{
  if (3 != argc) {
    std::cout << "Parameter missing" << std::endl;
  }

  std::string path(argv[1]);
  if ('/' == path.back()) {
    path.pop_back();
  }

  std::string ifile(path + "/" + argv[2]);
  std::string ofile(path + "/" + "out_" + argv[2]);

  std::cout << "Input file  : " << ifile << std::endl;
  std::cout << "Output file : " << ofile << std::endl;

  // Load sensor data from text file
  readDatFile(ifile);

  MerweScaledSigmaPoints sigmas = MerweScaledSigmaPoints(dim_x, alpha, beta, kappa);
  UnscentedKalmanFilter  ukf    = UnscentedKalmanFilter(dim_x, dim_z, dt, fx, hx, sigmas);

  // Set initial conditions
  double p0     = z_press[0];
  double height = 44330 * (1 - pow(p0 / 101325.0, 0.1902));

  ukf.x(0) = height; // Initial height
  ukf.x(1) = 0.0;        // Initial speed
  ukf.x(2) = 0.0;        // Initial acceleration

  // Initialize process state covariance matrix
  ukf.P(0, 0) = 1, ukf.P(0, 1) = 0, ukf.P(0, 2) = 0;
  ukf.P(1, 0) = 0, ukf.P(1, 1) = 1, ukf.P(1, 2) = 0;
  ukf.P(2, 0) = 0, ukf.P(2, 1) = 0, ukf.P(2, 2) = 1;

  // Set measurement covariance matrix (based on previous measurement)
  const double p_std = 3.74;
  const double a_std = 0.02;

  ukf.R(0, 0) = (p_std * p_std), ukf.R(0, 1) = 0.0;
  ukf.R(1, 0) = 0.0, ukf.R(1, 1) = (a_std * a_std);

  // Initialize process noise matrix
  ukf.Q(0, 0) = 1.2e-09, ukf.Q(0, 1) = 1.2e-07, ukf.Q(0, 2) = 6.0e-06;
  ukf.Q(1, 0) = 1.2e-07, ukf.Q(1, 1) = 1.2e-05, ukf.Q(1, 2) = 6.0e-04;
  ukf.Q(2, 0) = 6.0e-06, ukf.Q(2, 1) = 6.0e-04, ukf.Q(2, 2) = 3.0e-02;


  std::ofstream outfile(ofile);

  for (size_t i = 1; i < sampleCount; ++i) {
    Matrix z = Matrix(2, 1);
    z(0)     = z_press[i];
    z(1)     = z_acc[i];

    ukf.predict();
    ukf.update(z);

    out_height[i] = ukf.x(0);
    out_vario[i]  = ukf.x(1);
    out_acc[i]    = ukf.x(2);

    if (outfile.is_open()) {
      outfile << out_height[i] << " " << out_vario[i] << " " << out_acc[i] << std::endl;
    }
  }

  return 0;
}

/****************************** END OF FILE **********************************/
