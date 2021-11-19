
#include <kalmanfilter/Matrix.hpp>
#include <gtest/gtest.h>

class MatrixTest : public ::testing::Test {
  // clang-format off
  const double A_init[3][3] {
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0},
    {7.0, 8.0, 9.0}
  };
  // clang-format on

public:
  Matrix<3, 3> A;
  Matrix<3, 3> B;

  MatrixTest() : A(A_init), B()
  {
    B.fill(1.0);
  }

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

TEST_F(MatrixTest, Indexing)
{
  EXPECT_DOUBLE_EQ(1.0, A[0][0]);
  EXPECT_DOUBLE_EQ(2.0, A[0][1]);
  EXPECT_DOUBLE_EQ(3.0, A[0][2]);
  EXPECT_DOUBLE_EQ(4.0, A[1][0]);
  EXPECT_DOUBLE_EQ(5.0, A[1][1]);
  EXPECT_DOUBLE_EQ(6.0, A[1][2]);
  EXPECT_DOUBLE_EQ(7.0, A[2][0]);
  EXPECT_DOUBLE_EQ(8.0, A[2][1]);
  EXPECT_DOUBLE_EQ(9.0, A[2][2]);

  A[1][1] = 99.0;
  EXPECT_DOUBLE_EQ(99.0, A[1][1]);
}

TEST_F(MatrixTest, ConstIndexing)
{
  const auto M{A};
  EXPECT_DOUBLE_EQ(5.0, M[1][1]);
}

TEST_F(MatrixTest, Addition)
{
  auto S = A + B;

  EXPECT_EQ(A.rows(), S.rows());
  EXPECT_EQ(A.cols(), S.cols());

  for (size_t r = 0; r < S.rows(); ++r) {
    for (size_t c = 0; c < S.cols(); ++c) {
      EXPECT_DOUBLE_EQ(A[r][c] + B[r][c], S[r][c]);
    }
  }
}

TEST_F(MatrixTest, Subtraction)
{
  auto S = A - B;

  EXPECT_EQ(A.rows(), S.rows());
  EXPECT_EQ(A.cols(), S.cols());

  for (size_t r = 0; r < S.rows(); ++r) {
    for (size_t c = 0; c < S.cols(); ++c) {
      EXPECT_DOUBLE_EQ(A[r][c] - B[r][c], S[r][c]);
    }
  }
}

TEST_F(MatrixTest, Multiplication)
{
  // clang-format off
  double M_init[3][2] = {
    {1, 2},
    {3, 4},
    {5, 6}
  };

  double expected[3][2] = {
    { 22, 28, },
    { 49, 64, },
    { 76, 100,}
  };
  // clang-format on

  Matrix<3, 2> M(M_init);

  auto S = A * M;

  EXPECT_EQ(A.rows(), S.rows());
  EXPECT_EQ(M.cols(), S.cols());

  for (size_t r = 0; r < S.rows(); ++r) {
    for (size_t c = 0; c < S.cols(); ++c) {
      EXPECT_DOUBLE_EQ(expected[r][c], S[r][c]);
    }
  }
}

TEST_F(MatrixTest, ScalarMultiplicationFromLeft)
{
  double factor = 3.0;
  auto   S      = factor * A;

  EXPECT_EQ(A.rows(), S.rows());
  EXPECT_EQ(A.cols(), S.cols());

  for (size_t r = 0; r < S.rows(); ++r) {
    for (size_t c = 0; c < S.cols(); ++c) {
      double expected = A[r][c] * factor;
      EXPECT_DOUBLE_EQ(expected, S[r][c]);
    }
  }
}

TEST_F(MatrixTest, ScalarMultiplicationFromRight)
{
  double factor = 3.0;
  auto   S      = A * factor;

  EXPECT_EQ(A.rows(), S.rows());
  EXPECT_EQ(A.cols(), S.cols());

  for (size_t r = 0; r < S.rows(); ++r) {
    for (size_t c = 0; c < S.cols(); ++c) {
      double expected = A[r][c] * factor;
      EXPECT_DOUBLE_EQ(expected, S[r][c]);
    }
  }
}

TEST_F(MatrixTest, EqualityOperator_Self)
{
  EXPECT_TRUE(A == A);
}

TEST_F(MatrixTest, EqualityOperator_Match)
{
  auto C(A);

  EXPECT_TRUE(A == C);
}

TEST_F(MatrixTest, EqualityOperator_NoMatch)
{
  EXPECT_FALSE(A == B);
}

TEST_F(MatrixTest, Determinant)
{
  // clang-format off
  double M_init[3][3] = {
      {2, 9, 0},
      {7, 1, 1},
      {6, 7, 8},
  };
  // clang-format on

  Matrix<3, 3> M(M_init);

  EXPECT_DOUBLE_EQ(-448.0, M.det());
}

TEST_F(MatrixTest, Cofactor)
{
  // clang-format off
  double expected[2][2] = {
    { 5, 6,},
    { 8, 9.}
  };
  // clang-format on

  auto cf = A.cof(0, 0);

  EXPECT_EQ(A.rows() - 1, cf.rows());
  EXPECT_EQ(A.cols() - 1, cf.cols());

  for (size_t r = 0; r < cf.rows(); ++r) {
    for (size_t c = 0; c < cf.cols(); ++c) {
      EXPECT_DOUBLE_EQ(expected[r][c], cf[r][c]);
    }
  }
}

TEST_F(MatrixTest, Adjoint)
{
  // clang-format off
  const double M_init[4][4] = {
     5, -2, 2, 7,
     1,  0, 0, 3,
    -3,  1, 5, 0,
     3, -1,-9, 4,
  };

  const double expected[4][4] = {
    {-12,   76,  -60,  -36,},
    {-56,  208,  -82,  -58,},
    {  4,    4,   -2,  -10,},
    {  4,    4,   20,   12,},
  };
  // clang-format on

  Matrix<4, 4> M(M_init);

  auto adj = M.adj();

  for (size_t r = 0; r < adj.rows(); ++r) {
    for (size_t c = 0; c < adj.cols(); ++c) {
      EXPECT_DOUBLE_EQ(expected[r][c], adj[r][c]);
    }
  }
}

TEST_F(MatrixTest, AdjointOneElementMatrix)
{
  const double M_init[1][1] = {{1}};

  Matrix<1, 1> M(M_init);

  EXPECT_DOUBLE_EQ(1, M.adj()[0][0]);
}

TEST_F(MatrixTest, Inverse)
{
  // clang-format off
  double M_init[4][4] = {
     5, -2, 2, 7,
     1,  0, 0, 3,
    -3,  1, 5, 0,
     3, -1,-9, 4,
  };

  double expected[4][4] = {
    {-0.136364, 0.863636 , -0.681818 , -0.409091,},
    {-0.636364, 2.36364  , -0.931818 , -0.659091,},
    {0.0454545, 0.0454545, -0.0227273, -0.113636,},
    {0.0454545, 0.0454545,  0.227273 ,  0.136364,},
  };
  // clang-format on

  Matrix<4, 4> M(M_init);
  auto         inverse = M.inv();

  EXPECT_EQ(M.rows(), inverse.rows());
  EXPECT_EQ(M.cols(), inverse.cols());

  for (size_t r = 0; r < inverse.rows(); ++r) {
    for (size_t c = 0; c < inverse.cols(); ++c) {
      EXPECT_NEAR(expected[r][c], inverse[r][c], 0.0001);
    }
  }
}

TEST_F(MatrixTest, Transpose)
{
  // clang-format off
  double M_init[3][2] = {
    1, 2,
    3, 4,
    5, 6,
  };

  double expected[2][3] = {
    {1,3,5,},
    {2,4,6,},
  };
  // clang-format on

  Matrix<3, 2> M(M_init);
  auto         transpose = M.transpose();

  EXPECT_EQ(M.rows(), transpose.cols());
  EXPECT_EQ(M.cols(), transpose.rows());

  for (size_t r = 0; r < transpose.rows(); ++r) {
    for (size_t c = 0; c < transpose.cols(); ++c) {
      EXPECT_DOUBLE_EQ(expected[r][c], transpose[r][c]);
    }
  }
}

TEST_F(MatrixTest, CholeskySquareRoot)
{
  // clang-format off
  double M_init[3][3] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };

  double expected[3][3] = {
    { 2, 0, 0},
    { 6, 1, 0},
    {-8, 5, 3},
  };
  // clang-format on

  Matrix<3, 3> M(M_init);
  auto         sq = M.sqrt();

  EXPECT_EQ(M.rows(), sq.rows());
  EXPECT_EQ(M.cols(), sq.cols());

  for (size_t r = 0; r < sq.rows(); ++r) {
    for (size_t c = 0; c < sq.cols(); ++c) {
      EXPECT_DOUBLE_EQ(expected[r][c], sq[r][c]);
    }
  }
}

TEST_F(MatrixTest, Fill)
{
  A.fill(99.0);

  for (size_t i = 0; i < A.rows(); ++i) {
    for (size_t j = 0; j < A.cols(); ++j) {
      EXPECT_DOUBLE_EQ(99.0, A[i][j]);
    }
  }
}

TEST_F(MatrixTest, GetRowVector)
{
  // clang-format off
  double M_init[3][3] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };
  // clang-format on

  Matrix<3, 3> M(M_init);

  auto row0 = M.row(0);

  for (size_t i = 0; i < row0.size(); ++i) {
    EXPECT_DOUBLE_EQ(M[0][i], row0[i]);
  }
}

TEST_F(MatrixTest, GetColumnVector)
{
  // clang-format off
  double M_init[3][3] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };
  // clang-format on

  Matrix<3, 3> M(M_init);

  auto col1 = M.col(1);

  for (size_t i = 0; i < col1.size(); ++i) {
    EXPECT_DOUBLE_EQ(M[i][1], col1[i]);
  }
}

TEST_F(MatrixTest, RowToMatrixConversion)
{
  // clang-format off
  double M_init[3][3] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };
  // clang-format on

  Matrix<3, 3> M(M_init);

  auto row0 = M.row(0);

  Matrix<1, 3> Mrow0 = static_cast<Matrix<1, 3>>(row0);

  for(size_t c = 0; c < Mrow0.cols(); ++c) {
    EXPECT_DOUBLE_EQ(M[0][c], Mrow0[0][c]);
  }
}

TEST_F(MatrixTest, ColumnToMatrixConversion)
{
  // clang-format off
  double M_init[3][3] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };
  // clang-format on

  Matrix<3, 3> M(M_init);

  auto col1 = M.col(1);

  Matrix<3, 1> MCol1 = col1;

  for(size_t r = 0; r < MCol1.rows(); ++r) {
    EXPECT_DOUBLE_EQ(M[r][1], MCol1[r][0]);
  }
}
