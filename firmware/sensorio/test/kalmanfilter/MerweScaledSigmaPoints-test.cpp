#include <kalmanfilter/MerweScaledSigmaPoints.hpp>
#include <gtest/gtest.h>

class MerweScaledSigmas : public ::testing::Test {
public:
  const double alpha = 0.3;
  const double beta  = 2.0;
  const double kappa = 0.1;

  MerweScaledSigmaPoints<3> sigmas{alpha, beta, kappa};

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

TEST_F(MerweScaledSigmas, GenerateSigmas)
{
  double       x_values[3][1] = {107.9866, 0.0, 0.0};
  Matrix<3, 1> x{x_values};

  // clang-format off
  double P_values[3][3] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
  };

  double S_values[3][7] = {
    {107.9866, 108.5148, 107.9866, 107.9866, 107.4583, 107.9866 , 107.9866},
    {  0.0   ,   0.0   ,   0.5282,   0.0   ,   0.0   ,   -0.5282,   0.0},
    {  0.0   ,   0.0   ,   0.0   ,   0.5282,   0.0   ,    0.0   ,  -0.5282},
  };
  // clang-format on

  Matrix<3, 3> P{P_values};
  Matrix<3, 7> S_expected{S_values};

  auto S = sigmas.generateSigmas(x, P);

  for (size_t r = 0; r < S.rows(); ++r) {
    for (size_t c = 0; c < S.cols(); ++c) {
      EXPECT_NEAR(S_expected[r][c], S[r][c], 0.0001);
    }
  }
}

TEST_F(MerweScaledSigmas, CalculateWeights)
{
  double Wm0 = -9.7526;
  double Wc0 = -6.8426;
  double Wi  = 1.7921;

  EXPECT_NEAR(Wm0, sigmas.Wm[0][0], 0.0001);
  EXPECT_NEAR(Wc0, sigmas.Wc[0][0], 0.0001);

  for (size_t i = 1; i < MerweScaledSigmaPoints<3>::NUM_OF_SIGMAS; ++i) {
    EXPECT_NEAR(Wi, sigmas.Wc[i][0], 0.0001);
    EXPECT_NEAR(Wi, sigmas.Wm[i][0], 0.0001);
  }
}
