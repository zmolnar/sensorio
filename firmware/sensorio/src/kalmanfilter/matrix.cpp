/**
 * @file matrix.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "matrix.h"

#include <cmath>
#include <cstring>

#if defined(PCSIM)
#include <cassert>
#define ASSERT assert
#else
#define ASSERT
#endif

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
Matrix &Matrix::operator=(const Matrix &rhs)
{
  ASSERT(this->rows == rhs.rows);
  ASSERT(this->columns == rhs.columns);

  memcpy(this->items, rhs.items, sizeof(this->items));

  return *this;
}

double &Matrix::operator()(size_t i, size_t j) const
{
  ASSERT(i < this->rows);
  ASSERT(j < this->columns);

  return items[i * columns + j];
}

Matrix Matrix::operator+(const Matrix &rhs)
{
  ASSERT(this->rows == rhs.rows);
  ASSERT(this->columns == rhs.columns);

  Matrix r = Matrix(this->rows, this->columns);

  for (size_t i = 0; i < r.rows; ++i) {
    for (size_t j = 0; j < r.columns; ++j) {
      r(i, j) = (*this)(i, j) + rhs(i, j);
    }
  }

  return r;
}

Matrix Matrix::operator*(const Matrix &rhs)
{
  ASSERT(this->columns == rhs.rows);

  Matrix r = Matrix(this->rows, rhs.columns);

  for (size_t i = 0; i < r.rowCount(); ++i) {
    for (size_t j = 0; j < r.columnCount(); ++j) {
      double product = 0;
      for (size_t x = 0; x < this->columnCount(); ++x) {
        product += (*this)(i, x) * rhs(x, j);
      }

      r(i, j) = product;
    }
  }

  return r;
}

double Matrix::det(void)
{
  double D = 0.0;

  if (1 == this->itemCount)
    return items[0];

  int sign = 1;

  for (size_t r = 0; r < this->rows; ++r) {
    Matrix cof = this->cof(0, r);
    D += sign * (*this)(0, r) * cof.det();

    sign = -sign;
  }

  return D;
}

Matrix Matrix::cof(size_t row, size_t column)
{
  Matrix cof = Matrix(this->rows - 1, this->columns - 1);

  size_t ri = 0;
  size_t ci = 0;
  for (size_t r = 0; r < this->rows; ++r) {
    for (size_t c = 0; c < this->columns; ++c) {
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
  Matrix adj = Matrix(this->columns, this->rows);

  if (1 == this->itemCount) {
    adj(0, 0) = 1;
    return adj;
  }

  for (size_t r = 0; r < this->rows; ++r) {
    for (size_t c = 0; c < this->columns; ++c) {
      Matrix cof = this->cof(r, c);
      int    s   = ((r + c) % 2 == 0) ? 1 : -1;
      adj(c, r)  = (s)*cof.det();
    }
  }

  return adj;
}

Matrix Matrix::inv(void)
{
  ASSERT(this->rows == this->columns);

  double det = this->det();
  ASSERT(det != 0);

  Matrix adj = this->adj();

  Matrix inv = Matrix(this->rows, this->columns);

  for (size_t i = 0; i < inv.rows; ++i)
    for (size_t j = 0; j < inv.columns; ++j)
      inv(i, j) = adj(i, j) / det;

  return inv;
}

Matrix Matrix::T(void)
{
  Matrix trsp = Matrix(this->columns, this->rows);

  for (size_t r = 0; r < this->rows; ++r) {
    for (size_t c = 0; c < this->columns; ++c) {
      trsp(c, r) = (*this)(r, c);
    }
  }

  return trsp;
}

Matrix Matrix::sqrt(void)
{
  ASSERT(this->rows == this->columns);
  
  Matrix lower = Matrix(this->rows, this->columns);

  for (size_t r = 0; r < this->rows; ++r) {
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

#if defined(PCSIM)
std::ostream &operator<<(std::ostream &os, const Matrix &m)
{
  os << std::endl << "[";

  for (size_t i = 0; i < m.rowCount(); ++i) {
    if (0 < i)
      os << std::endl << " ";

    for (size_t j = 0; j < m.columnCount(); ++j) {
      os << m(i, j);
      if (j < (m.columnCount() - 1))
        os << "\t";
    }
  }

  os << "]" << std::endl;

  return os;
}
#endif

/****************************** END OF FILE **********************************/
