#include <gtest/gtest.h>
#include <MerweScaledSigmaPoints.h>

class MerweScaledSigmas : public ::testing::Test {
public:
  const size_t dim_x = 3;
  const double alpha = 0.3;
  const double beta  = 2.0;
  const double kappa = 0.1;

  MerweScaledSigmaPoints sigmas;

  MerweScaledSigmas() : sigmas(dim_x, alpha, beta, kappa)
  {
  }

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

TEST_F(MerweScaledSigmas, NumOfSigmas) 
{
  EXPECT_EQ(7, sigmas.numOfSigmas());
}

TEST_F(MerweScaledSigmas, GenerateSigmas)
{
  double x_values[3] = {107.9866, 0.0, 0.0};
  Matrix x(3, 1);
  x.load(x_values, 3);

  double P_values[9] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
  };
  Matrix P(3, 3);
  P.load(P_values, 9);

  double S_values[] = {
    107.9866,  0.0     ,  0.0,
    108.5148,  0.0     ,  0.0,
    107.9866,  0.5282  ,  0.0,
    107.9866,  0.0     ,  0.5282,
    107.4583,  0.0     ,  0.0,
    107.9866, -0.5282  ,  0.0,
    107.9866,  0.0     , -0.5282,
  };
  Matrix S_expected(7, 3);
  S_expected.load(S_values, 21);


  Matrix S = sigmas.generateSigmas(x, P);

  for(size_t i = 0; i < S.rows; ++i) {
    for(size_t j = 0; j < S.columns; ++j) {
      EXPECT_NEAR(S_expected(i, j), S(i, j), 0.0001);
    }
  }
}

TEST_F(MerweScaledSigmas, CalculateWeights)
{
  double Wm0 = -9.7526;
  double Wc0 = -6.8426;
  double Wi  = 1.7921;

  EXPECT_NEAR(Wm0, sigmas.Wm(0), 0.0001);
  EXPECT_NEAR(Wc0, sigmas.Wc(0), 0.0001);

  for(size_t i = 1; i < sigmas.numOfSigmas(); ++i) {
    EXPECT_NEAR(Wi, sigmas.Wc(i), 0.0001);
    EXPECT_NEAR(Wi, sigmas.Wm(i), 0.0001);
  }
}