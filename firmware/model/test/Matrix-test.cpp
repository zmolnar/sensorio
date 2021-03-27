
#include "Matrix.h"
#include "gtest/gtest.h"

class MatrixTest : public ::testing::Test {
public:
  Matrix A;
  Matrix B;

  MatrixTest() : A(3, 3), B(3, 3)
  {
  }

  virtual void SetUp()
  {
    double A_values[] = {
      1.0, 2.0, 3.0, 
      4.0, 5.0, 6.0, 
      7.0, 8.0, 9.0
    };
    A.load(A_values, 9);

    B.fill(1.0);
  }

  virtual void TearDown()
  {
  }
};

TEST_F(MatrixTest, Indexing)
{
  EXPECT_DOUBLE_EQ(5.0, A(1, 1));
  
  A(1, 1) = 99.0;
  EXPECT_DOUBLE_EQ(99.0, A(1, 1));
}

TEST_F(MatrixTest, ConstIndexing)
{
  const Matrix &M = A;
  EXPECT_DOUBLE_EQ(5.0, M(1, 1));
}

TEST_F(MatrixTest, Addition)
{
  Matrix S = A + B;

  for (size_t i = 0; i < S.rows; ++i) {
    for (size_t j = 0; j < S.columns; ++j) {
      int expected = A(i, j) + B(i, j);
      EXPECT_DOUBLE_EQ(expected, S(i, j));
    }
  }
}

TEST_F(MatrixTest, Subtraction)
{
  Matrix S = A - B;

  for (size_t i = 0; i < S.rows; ++i) {
    for (size_t j = 0; j < S.columns; ++j) {
      int expected = A(i, j) - B(i, j);
      EXPECT_DOUBLE_EQ(expected, S(i, j));
    }
  }
}

TEST_F(MatrixTest, Multiplication)
{
  double M_values[] = {
    1, 2,
    3, 4,
    5, 6
  };
  Matrix M(3, 2);
  M.load(M_values, 6);

  double expected[3][2] = {
    { 22, 28, },
    { 49, 64, },
    { 76, 100,}
  };

  Matrix S = A * M;

  for (size_t i = 0; i < S.rows; ++i) {
    for (size_t j = 0; j < S.columns; ++j) {
      EXPECT_DOUBLE_EQ(expected[i][j], S(i, j));
    }
  }
}

TEST_F(MatrixTest, ScalarMultiplicationFromLeft)
{
  double factor = 3.0;
  Matrix S(3, 3);

  S = factor * A;
  for (size_t i = 0; i < S.rows; ++i) {
    for (size_t j = 0; j < S.columns; ++j) {
      double expected = A(i, j) * factor;
      EXPECT_DOUBLE_EQ(expected, S(i, j));
    }
  }
}

TEST_F(MatrixTest, ScalarMultiplicationFromRight)
{
  double factor = 3.0;
  Matrix S(3, 3);

  S = A * factor;
  for (size_t i = 0; i < S.rows; ++i) {
    for (size_t j = 0; j < S.columns; ++j) {
      double expected = A(i, j) * factor;
      EXPECT_DOUBLE_EQ(expected, S(i, j));
    }
  }
}

TEST_F(MatrixTest, Determinant)
{
  double M_values[] = {
      2, 9, 0,
      7, 1, 1,
      6, 7, 8,
  };
  Matrix M(3, 3);
  M.load(M_values, 9);

  double det = M.det();
  EXPECT_DOUBLE_EQ(-448.0, det);
}

TEST_F(MatrixTest, Cofactor)
{
  double expected[2][2] = {
    { 5, 6,},
    { 8, 9.}
  };

  Matrix Cf = A.cof(0, 0);

  for (size_t i = 0; i < Cf.rows; ++i) {
    for (size_t j = 0; j < Cf.columns; ++j) {
      EXPECT_DOUBLE_EQ(expected[i][j], Cf(i, j));
    }
  }
}

TEST_F(MatrixTest, Adjoint)
{
  double M_values[] = {
    5, -2, 2, 7,  
    1,  0, 0, 3,
    -3, 1, 5, 0,
    3, -1,-9, 4,
  };

  double expected[4][4] = {
    {-12,   76,  -60,  -36,},
    {-56,  208,  -82,  -58,},
    {  4,    4,   -2,  -10,},
    {  4,    4,   20,   12,}, 
  };

  Matrix M(4, 4);
  M.load(M_values, 16);

  Matrix Adj = M.adj();

  for (size_t i = 0; i < Adj.rows; ++i) {
    for (size_t j = 0; j < Adj.columns; ++j) {
      EXPECT_DOUBLE_EQ(expected[i][j], Adj(i, j));
    }
  }
}

TEST_F(MatrixTest, Inverse)
{
  double M_values[] = {
    5, -2, 2, 7,  
    1,  0, 0, 3,
    -3, 1, 5, 0,
    3, -1,-9, 4,
  };

  double expected[4][4] = {
    {-0.136364, 0.863636 , -0.681818 , -0.409091,}, 
    {-0.636364, 2.36364  , -0.931818 , -0.659091,}, 
    {0.0454545, 0.0454545, -0.0227273, -0.113636,}, 
    {0.0454545, 0.0454545,  0.227273 ,  0.136364,},
  };

  Matrix M(4, 4);
  M.load(M_values, 16);
  Matrix Inv = M.inv();

  for (size_t i = 0; i < Inv.rows; ++i) {
    for (size_t j = 0; j < Inv.columns; ++j) {
      EXPECT_NEAR(expected[i][j], Inv(i, j), 0.0001);
    }
  }
}

TEST_F(MatrixTest, Transpose)
{
  double M_values[] = {
    1, 2,
    3, 4,
    5, 6,
  };
  Matrix M(3,2);
  M.load(M_values, 6);

  double expected[2][3] = {
    {1,3,5,},
    {2,4,6,},
  };

  Matrix Tr = M.T();

  for (size_t i = 0; i < Tr.rows; ++i) {
    for (size_t j = 0; j < Tr.columns; ++j) {
      EXPECT_DOUBLE_EQ(expected[i][j], Tr(i, j));
    }
  }
}

TEST_F(MatrixTest, CholeskySquareRoot)
{
  double M_values[] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };
  Matrix M(3,3);
  M.load(M_values, 9);

  double expected[3][3] = {
    { 2, 0, 0},
    { 6, 1, 0},
    {-8, 5, 3},
  };

  Matrix Sq = M.sqrt();

  for (size_t i = 0; i < Sq.rows; ++i) {
    for (size_t j = 0; j < Sq.columns; ++j) {
      EXPECT_DOUBLE_EQ(expected[i][j], Sq(i, j));
    }
  }
}

TEST_F(MatrixTest, GetRowVector)
{
  double M_values[] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };
  Matrix M(3,3);
  M.load(M_values, 9);

  Vector row0 = M.row(0);

  EXPECT_EQ(1, row0.rows);
  EXPECT_EQ(3, row0.columns);
  EXPECT_EQ(3, row0.length);

  for (size_t i = 0; i < row0.length; ++i) {
    EXPECT_DOUBLE_EQ(M(0, i), row0(i));
  }
}

TEST_F(MatrixTest, GetColumnVector)
{
  double M_values[] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };
  Matrix M(3,3);
  M.load(M_values, 9);

  Vector col1 = M.column(1);

  EXPECT_EQ(3, col1.rows);
  EXPECT_EQ(1, col1.columns);
  EXPECT_EQ(3, col1.length);

  for (size_t i = 0; i < col1.length; ++i) {
    EXPECT_DOUBLE_EQ(M(i, 1), col1(i));
  }
}

TEST_F(MatrixTest, Fill)
{
  A.fill(99.0);

  for (size_t i = 0; i < A.rows; ++i) {
    for (size_t j = 0; j < A.columns; ++j) {
      EXPECT_DOUBLE_EQ(99.0, A(i, j));
    }
  }
}

TEST_F(MatrixTest, Load)
{
  double values[9] = {
      4,  12, -16,
     12,  37, -43,
    -16, -43,  98,
  };

  A.load(values, 9);

  for (size_t i = 0; i < A.rows; ++i) {
    for (size_t j = 0; j < A.columns; ++j) {
      EXPECT_DOUBLE_EQ(values[i * 3 + j], A(i, j));
    }
  }
}