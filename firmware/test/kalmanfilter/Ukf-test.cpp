#include <kalmanfilter/MerweScaledSigmaPoints.hpp>
#include <kalmanfilter/Ukf.hpp>
#include <gtest/gtest.h>

#include <cmath>

// State transition function
const typename UnscentedKalmanFilter<3,2>::StateVector fx(
  const typename UnscentedKalmanFilter<3,2>::StateVector &x, double dt)
{
  typename UnscentedKalmanFilter<3,2>::StateVector xout {};

  // Position     = a*dt2/2 + v*dt + x0
  // Speed        = a*dt + v
  // Acceleration = a

  xout[0][0] = (x[2][0] * dt * dt / 2) + (x[1][0] * dt) + x[0][0];
  xout[1][0] = (x[2][0] * dt) + x[1][0];
  xout[2][0] = x[2][0];

  return xout;
}

// Measurement function
typename UnscentedKalmanFilter<3,2>::MeasurementVector hx(
  const typename UnscentedKalmanFilter<3,2>::StateVector &x)
{
  // Convert height to pressure at altitude using barometric formula
  double press = 101325 * pow((1 - 2.25577 * pow(10, -5) * x[0][0]), 5.25588);
  double acc   = x[2][0];

  typename UnscentedKalmanFilter<3,2>::MeasurementVector zout{};
  zout[0][0] = press;
  zout[1][0] = acc;

  return zout;
}

class UkfTest : public ::testing::Test {
public:
  static constexpr size_t dim_x = 3;
  static constexpr size_t dim_z = 2;
  const double dt    = 0.02;
  const double alpha = 0.3;
  const double beta  = 2.0;
  const double kappa = 0.1;

  MerweScaledSigmaPoints<dim_x> sigmas;
  UnscentedKalmanFilter<dim_x, dim_z> ukf;

  UkfTest() : sigmas(alpha, beta, kappa), ukf(fx, hx, sigmas)
  {
    // Set initial conditions
    ukf.x[0][0] = 107.9866; // Initial height
    ukf.x[1][0] = 0.0;      // Initial speed
    ukf.x[2][0] = 0.0;      // Initial acceleration

    // Initialize process state covariance matrix
    ukf.P[0][0] = 1, ukf.P[0][1] = 0, ukf.P[0][2] = 0;
    ukf.P[1][0] = 0, ukf.P[1][1] = 1, ukf.P[1][2] = 0;
    ukf.P[2][0] = 0, ukf.P[2][1] = 0, ukf.P[2][2] = 1;

    // Set measurement covariance matrix (based on previous measurement)
    const double p_std = 3.74;
    const double a_std = 0.02;

    ukf.R[0][0] = (p_std * p_std), ukf.R[0][1] = 0.0;
    ukf.R[1][0] = 0.0, ukf.R[1][1] = (a_std * a_std);

    // Initialize process noise matrix
    ukf.Q[0][0] = 1.2e-09, ukf.Q[0][1] = 1.2e-07, ukf.Q[0][2] = 6.0e-06;
    ukf.Q[1][0] = 1.2e-07, ukf.Q[1][1] = 1.2e-05, ukf.Q[1][2] = 6.0e-04;
    ukf.Q[2][0] = 6.0e-06, ukf.Q[2][1] = 6.0e-04, ukf.Q[2][2] = 3.0e-02;
  }

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

TEST_F(UkfTest, Predict)
{
  double x_expected[3] = {
    107.9866, 3.8163e-17, 0.0,
  };
  double P_expected[3][3] = {
    1.0004,     2.000412e-02, 2.06e-04,
    0.02000412, 1.000412,     0.0206,
    2.06e-04,   2.06e-02,     1.03
  };

  ukf.predict(0.02);

  // Test state prediction
  for(size_t i = 0; i < 3; ++i) {
    EXPECT_NEAR(x_expected[i], ukf.x_prior[i][0], 1e-07);
  }

  // Test state covariance matrix
  for(size_t i = 0; i < 3; ++ i) {
    for(size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(P_expected[i][j], ukf.P_prior[i][j], 1e-07);
    }
  }
}

TEST_F(UkfTest, Update)
{
  // clang-format off
  double x_expected[3] = {
    107.793772,
    -0.003463,
    0.019599,
  };

  double P_expected[3][3] = {
    1.016878e-01, 2.033063e-03, 6.00812e-06,
    2.033063e-03, 9.996529e-01, 6.079953e-04,
    6.008128e-06, 6.079953e-04, 3.039984e-02,
  };

  double K_expected[3][2] = {
    -7.556103e-02, 2.032130e-05,
    -0.00151062,   0.01998841,
    -6.040049e-09, 9.996001e-01,
  };
  // clang-format on

  Matrix<2,1> z{};
  z[0][0] = 100037;
  z[1][0] = 0.0196;

  ukf.predict(0.02);
  ukf.update(z);

  for(size_t i = 0; i < 3; ++i) {
    EXPECT_NEAR(x_expected[i], ukf.x[i][0], 1e-02);
  }

  // Test state covariance matrix
  for(size_t i = 0; i < 3; ++ i) {
    for(size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(P_expected[i][j], ukf.P[i][j], 1e-01);
    }
  }

  // Test Kalman gain
  for(size_t i = 0; i < 3; ++ i) {
    for(size_t j = 0; j < 2; ++j) {
      EXPECT_NEAR(K_expected[i][j], ukf.K[i][j], 1e-03);
    }
  }
}
