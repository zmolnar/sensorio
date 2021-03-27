/**
 * @file matrix.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Matrix.h"

#include <cmath>
#include <cstring>

#include "env.h"

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

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
Vector::Vector(Matrix &m) :
    matrix(m),
    rows(m.rows),
    columns(m.columns),
    length(m.rows * m.columns),
    index(0)
{
  ASSERT((1 == rows) || (1 == columns));
}

Vector &Vector::operator=(const Vector &v)
{
  if (1 == rows) {
    ASSERT(columns == (v.length));
    for (size_t i = 0; i < columns; ++i) {
      matrix(index, i) = v(i);
    }
  } else {
    ASSERT(rows == (v.length));
    for (size_t i = 0; i < rows; ++i) {
      matrix(i, index) = v(i);
    }
  }

  return (*this);
}

double &Vector::operator()(size_t i)
{
  if (1 == rows) {
    ASSERT(i < columns);
    return matrix(index, i);
  } else {
    ASSERT(i < rows);
    return matrix(i, index);
  }
}

double Vector::operator()(size_t i) const
{
  if (1 == rows) {
    ASSERT(i < columns);
    return matrix(index, i);
  } else {
    ASSERT(i < rows);
    return matrix(i, index);
  }
}

Matrix Vector::operator*(double c) const
{
  Matrix result = Matrix(rows, columns);
  Vector v(result);

  for(size_t i = 0; i < length; ++i) {
     v(i) = (*this)(i) * c;
  }

  return result;
}

Matrix Vector::operator+(const Vector &v) const
{
  ASSERT(length == v.length);
  Matrix result = Matrix(rows, columns);
  result.fill(0.0);

  if (1 == rows) {
    for (size_t i = 0; i < columns; ++i) {
      result(0, i) = (*this)(i) + v(i);
    }
  } else {
    for (size_t i = 0; i < rows; ++i) {
      result(i, 0) = (*this)(i) + v(i);
    }
  }

  return result;
}

Matrix Vector::operator-(const Vector &v) const
{
  ASSERT(length == v.length);
  Matrix result = Matrix(rows, columns);

  if (1 == rows) {
    for (size_t i = 0; i < columns; ++i) {
      result(0, i) = (*this)(i)-v(i);
    }
  } else {
    for (size_t i = 0; i < rows; ++i) {
      result(i, 0) = (*this)(i)-v(i);
    }
  }

  return result;
}

double Vector::dot(const Vector &v) const
{
  ASSERT((rows * columns) == (v.rows * v.columns));

  double product = 0.0;
  size_t length  = rows * columns;
  for (size_t i = 0; i < length; ++i) {
    product += (*this)(i)*v(i);
  }

  return product;
}
Matrix Vector::outer(const Vector &v) const
{
  size_t rowCount    = rows * columns;
  size_t columnCount = v.rows * v.columns;
  Matrix result      = Matrix(rowCount, columnCount);

  for (size_t i = 0; i < result.rows; ++i) {
    for (size_t j = 0; j < result.columns; ++j) {
      result(i, j) = (*this)(i)*v(j);
    }
  }

  return result;
}

Matrix::Matrix(const Matrix &m) :
    itemCount(m.itemCount), rows(m.rows), columns(m.columns)
{
  if (m.items) {
    items = (double *)malloc(itemCount * sizeof(double));
    ASSERT(items);
    memcpy(this->items, m.items, this->itemCount * sizeof(double));
  }
}

Matrix &Matrix::operator=(const Matrix &rhs)
{
  if (this != &rhs) {
    if (itemCount != rhs.itemCount) {
      free(items);
      items = (double *)malloc(rhs.itemCount * sizeof(double));
      ASSERT(items);
    }

    itemCount = rhs.itemCount;
    rows      = rhs.rows;
    columns   = rhs.columns;
    memcpy(items, rhs.items, itemCount * sizeof(double));
  }

  return *this;
}

double &Matrix::operator()(size_t i, size_t j)
{
  ASSERT(i < rows);
  ASSERT(j < columns);

  return items[i * columns + j];
}

double Matrix::operator()(size_t i, size_t j) const
{
  ASSERT(i < rows);
  ASSERT(j < columns);

  return items[i * columns + j];
}

Matrix Matrix::operator+(const Matrix &rhs) const
{
  ASSERT(rows == rhs.rows);
  ASSERT(columns == rhs.columns);

  Matrix r = Matrix(rows, columns);

  for (size_t i = 0; i < r.rows; ++i) {
    for (size_t j = 0; j < r.columns; ++j) {
      r(i, j) = (*this)(i, j) + rhs(i, j);
    }
  }

  return r;
}

Matrix Matrix::operator-(const Matrix &rhs) const
{
  ASSERT(rows == rhs.rows);
  ASSERT(columns == rhs.columns);

  Matrix r = Matrix(rows, columns);

  for (size_t i = 0; i < r.rows; ++i) {
    for (size_t j = 0; j < r.columns; ++j) {
      r(i, j) = (*this)(i, j) - rhs(i, j);
    }
  }

  return r;
}

Matrix Matrix::operator*(const Matrix &rhs) const
{
  ASSERT(columns == rhs.rows);

  Matrix prod = Matrix(rows, rhs.columns);

  for (size_t i = 0; i < prod.rows; ++i) {
    for (size_t j = 0; j < prod.columns; ++j) {
      double product = 0;
      for (size_t x = 0; x < columns; ++x) {
        product += (*this)(i, x) * rhs(x, j);
      }

      prod(i, j) = product;
    }
  }

  return prod;
}

Matrix Matrix::operator*(const double c) const
{
  Matrix prod = Matrix(rows, columns);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      prod(i, j) = c * (*this)(i, j);
    }
  }

  return prod;
}

Vector Matrix::row(size_t i)
{
  return Vector(*this, 1, columns, i);
}

Vector Matrix::column(size_t i)
{
  return Vector(*this, rows, 1, i);
}

double Matrix::det(void)
{
  double D = 0.0;

  if (1 == itemCount)
    return items[0];

  int sign = 1;

  for (size_t r = 0; r < rows; ++r) {
    Matrix cof = this->cof(0, r);
    D += sign * (*this)(0, r) * cof.det();

    sign = -sign;
  }

  return D;
}

Matrix Matrix::cof(size_t row, size_t column)
{
  Matrix cof = Matrix(rows - 1, columns - 1);

  size_t ri = 0;
  size_t ci = 0;
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {
      if ((r != row) && (c != column)) {
        cof(ri, ci) = (*this)(r, c);
        if (ci < (cof.columns - 1)) {
          ++ci;
        } else {
          ci = 0;
          ++ri;
        }
      }
    }
  }

  return cof;
}

Matrix Matrix::adj(void)
{
  // Note here, that the output must be transposed!
  Matrix adj = Matrix(columns, rows);

  if (1 == itemCount) {
    adj(0, 0) = 1;
    return adj;
  }

  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {
      Matrix cof = this->cof(r, c);
      int    s   = ((r + c) % 2 == 0) ? 1 : -1;
      adj(c, r)  = (s)*cof.det();
    }
  }

  return adj;
}

Matrix Matrix::inv(void)
{
  ASSERT(rows == columns);

  double det = this->det();
  ASSERT(det != 0);

  Matrix adj = this->adj();

  Matrix inv = Matrix(rows, columns);

  for (size_t i = 0; i < inv.rows; ++i)
    for (size_t j = 0; j < inv.columns; ++j)
      inv(i, j) = adj(i, j) / det;

  return inv;
}

Matrix Matrix::T(void)
{
  Matrix trsp = Matrix(columns, rows);

  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {
      trsp(c, r) = (*this)(r, c);
    }
  }

  return trsp;
}

Matrix Matrix::sqrt(void)
{
  ASSERT(rows == columns);

  Matrix lower = Matrix(rows, columns);

  for (size_t r = 0; r < rows; ++r) {
    // Off-diagonal elements
    for (size_t k = 0; k < r; ++k) {
      double value = (*this)(r, k);
      for (size_t j = 0; j < k; ++j)
        value -= lower(r, j) * lower(k, j);
      lower(r, k) = value / lower(k, k);
    }

    // Diagonal element
    double value = (*this)(r, r);
    for (size_t j = 0; j < r; ++j)
      value -= lower(r, j) * lower(r, j);
    lower(r, r) = std::sqrt(value);
  }

  return lower;
}

Matrix &Matrix::fill(double v)
{
  for (size_t i = 0; i < itemCount; ++i) {
    items[i] = v;
  }

  return (*this);
}

Matrix &Matrix::load(double array[], size_t length)
{
  ASSERT(itemCount == length);

  memcpy(items, array, itemCount * sizeof(double));

  return (*this);
}

Matrix operator*(double c, Matrix &rhs)
{
  return rhs * c;
}

#if defined(PCSIM)
std::ostream &operator<<(std::ostream &os, const Matrix &m)
{
  os << std::endl << "[";

  for (size_t i = 0; i < m.rows; ++i) {
    if (0 < i)
      os << std::endl << " ";

    for (size_t j = 0; j < m.columns; ++j) {
      os << m(i, j);
      if (j < (m.columns - 1))
        os << "\t";
    }
  }

  os << "]" << std::endl;

  return os;
}
#endif

/****************************** END OF FILE **********************************/
