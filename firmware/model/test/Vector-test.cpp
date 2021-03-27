
#include <gtest/gtest.h>
#include <matrix.h>

class VectorTest : public ::testing::Test {
public:
  Matrix M;

  VectorTest() : M(3, 3)
  {
  }

  virtual void SetUp()
  {
    double M_values[] = {
        4,    12, -16,
        12,   37, -43,
        -16, -43,  98,
    };

    M.load(M_values, 9);
  }

  virtual void TearDown()
  {
  }
};

TEST_F(VectorTest, Constructor)
{
  Vector v(M, 1, 3, 1);

  EXPECT_EQ(&M, &v.matrix);
  EXPECT_EQ(1, v.rows);
  EXPECT_EQ(3, v.columns);
  EXPECT_EQ(3, v.length);
}

TEST_F(VectorTest, MatrixConversionConstructor)
{
  Matrix Row(1, 7);
  Vector v(Row);

  EXPECT_EQ(&Row, &v.matrix);
  EXPECT_EQ(1, v.rows);
  EXPECT_EQ(7, v.columns);
  EXPECT_EQ(7, v.length);
}

TEST_F(VectorTest, RowIndexing)
{
  Vector v(M, 1, 3, 1);

  for (size_t i = 0; i < v.length; ++i) {
    EXPECT_DOUBLE_EQ(M(1, i), v(i));
  }
}

TEST_F(VectorTest, ColumnIndexing)
{
  Vector v(M, 3, 1, 1);

  for (size_t i = 0; i < v.length; ++i) {
    EXPECT_DOUBLE_EQ(M(i, 1), v(i));
  }
}


TEST_F(VectorTest, ColumnAssignment)
{
  Vector v = M.column(1);

  double N_values[] = {
    0, 0, 0, 
  };    
  Matrix N(3, 1);
  N.load(N_values, 3);

  v = Vector(N);

  for (size_t i = 0; i < v.length; ++i) {
    EXPECT_DOUBLE_EQ(N(i, 0), M(i, 1));
  }
}

TEST_F(VectorTest, RowAssignment)
{
  Vector v = M.row(1);

  double N_values[] = {
    0, 0, 0, 
  };    
  Matrix N(3, 1);
  N.load(N_values, 3);

  v = Vector(N);

  for (size_t i = 0; i < v.length; ++i) {
    EXPECT_DOUBLE_EQ(N(i, 0), M(1, i));
  }
}

TEST_F(VectorTest, RowAddition)
{
  double N_values[] = {
    1, 2, 3, 
  };    
  Matrix N(3, 1);
  N.load(N_values, 3);

  Vector v1 = M.row(1);
  Vector v2(N);

  Matrix S = v1 + v2;

  EXPECT_EQ(1, S.rows);
  EXPECT_EQ(3, S.columns);

  for (size_t i = 0; i < S.columns; ++i) {
    EXPECT_DOUBLE_EQ(v1(i) + v2(i), S(0, i));
  }
}

TEST_F(VectorTest, ColumnAddition)
{
  double N_values[] = {
    1, 2, 3, 
  };    
  Matrix N(3, 1);
  N.load(N_values, 3);

  Vector v1 = M.column(1);
  Vector v2(N);

  Matrix S = v1 + v2;

  EXPECT_EQ(3, S.rows);
  EXPECT_EQ(1, S.columns);

  for (size_t i = 0; i < S.rows; ++i) {
    EXPECT_DOUBLE_EQ(v1(i) + v2(i), S(i, 0));
  }
}

TEST_F(VectorTest, RowSubtraction)
{
  double N_values[] = {
    1, 2, 3, 
  };    
  Matrix N(3, 1);
  N.load(N_values, 3);

  Vector v1 = M.row(1);
  Vector v2(N);

  Matrix S = v1 - v2;

  EXPECT_EQ(1, S.rows);
  EXPECT_EQ(3, S.columns);

  for (size_t i = 0; i < S.columns; ++i) {
    EXPECT_DOUBLE_EQ(v1(i) - v2(i), S(0, i));
  }
}

TEST_F(VectorTest, ColumnSubtraction)
{
  double N_values[] = {
    1, 2, 3, 
  };    
  Matrix N(3, 1);
  N.load(N_values, 3);

  Vector v1 = M.column(1);
  Vector v2(N);

  Matrix S = v1 - v2;

  EXPECT_EQ(3, S.rows);
  EXPECT_EQ(1, S.columns);

  for (size_t i = 0; i < S.rows; ++i) {
    EXPECT_DOUBLE_EQ(v1(i) - v2(i), S(i, 0));
  }
}

TEST_F(VectorTest, DotProduct)
{
  double P_values[] = {
    1, 2, 3, 
  };    
  Matrix P(3, 1);
  P.load(P_values, 3);

  double R_values[] = {
    1, 2, 3, 
  };    
  Matrix R(3, 1);
  R.load(R_values, 3);

  double expected = 0.0;
  for(size_t i = 0; i < sizeof(P_values)/sizeof(P_values[0]); ++i)
    expected += P_values[i] * R_values[i];

  double dotproduct = Vector(P).dot(R);
  EXPECT_EQ(expected, dotproduct);
}

TEST_F(VectorTest, OuterProduct)
{
  double P_values[] = {
    1, 2, 3, 
  };    
  Matrix P(3, 1);
  P.load(P_values, 3);

  double R_values[] = {
    4, 5, 6, 
  };    
  Matrix R(3, 1);
  R.load(R_values, 3);

  Matrix outerproduct = Vector(P).outer(R);

  EXPECT_EQ(3, outerproduct.rows);
  EXPECT_EQ(3, outerproduct.columns);

  for(size_t i = 0; i < outerproduct.rows; ++i) {
    for (size_t j = 0; j < outerproduct.columns; ++j) {
      double expected = P_values[i] * R_values[j];
      EXPECT_DOUBLE_EQ(expected, outerproduct(i, j));
    }
  }
}