/**
 * @file matrix.h
 * @brief
 */

#ifndef MATRIX_H
#define MATRIX_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "env.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
class Matrix;
class Vector {
public:
  Matrix &matrix;
  size_t  rows;
  size_t  columns;
  size_t  length;
  size_t  index;

  Vector(Matrix &m, size_t r, size_t c, size_t i) :
      matrix(m), rows(r), columns(c), length(r * c), index(i)
  {
    ASSERT((1 == rows) || (1 == columns));
  }
  Vector(Matrix &m);
  ~Vector()
  {
  }

  Vector &operator=(const Vector &v);
  double &operator()(size_t i);
  double  operator()(size_t i) const;
  Matrix  operator+(const Vector &v) const;
  Matrix  operator-(const Vector &v) const;

  double dot(const Vector &v) const;
  Matrix outer(const Vector &v) const;
};

class Matrix {
  size_t  itemCount;
  double *items;

public:
  size_t rows;
  size_t columns;

  Matrix(size_t rows, size_t columns) :
      itemCount(rows * columns), rows(rows), columns(columns)
  {
    items = (double *)malloc(itemCount * sizeof(double));
    ASSERT(items);
    memset(items, 0, itemCount * sizeof(double));
  }

  Matrix(const Matrix &m);

  ~Matrix()
  {
    free(items);
  }

  Matrix &operator=(const Matrix &rhs);
  double &operator()(size_t i, size_t j = 0);
  double  operator()(size_t i, size_t j = 0) const;
  Matrix  operator+(const Matrix &rhs) const;
  Matrix  operator-(const Matrix &rhs) const;
  Matrix  operator*(const Matrix &rhs) const;
  Matrix  operator*(const double c) const;

  Vector row(size_t i);
  Vector column(size_t i);

  double det(void);
  Matrix cof(size_t row, size_t column);
  Matrix adj(void);
  Matrix inv(void);
  Matrix T(void);
  Matrix sqrt(void);

  Matrix &fill(double v);
  Matrix &load(double array[], size_t length);
};

Matrix operator*(double c, Matrix &rhs);

#if defined(PCSIM)
#include <iostream>
std::ostream &operator<<(std::ostream &os, const Matrix &m);
#endif

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* MATRIX_H */

/****************************** END OF FILE **********************************/
