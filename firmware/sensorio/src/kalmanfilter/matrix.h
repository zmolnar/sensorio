/**
 * @file matrix.h
 * @brief
 */

#ifndef MATRIX_H
#define MATRIX_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(PCSIM)
#include <iostream>
#endif

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
class Matrix
{
  const size_t itemCount;
  size_t rows;
  size_t columns;
  double *items;
  public:
    Matrix(const size_t rowCount, const size_t columnCount):
      itemCount(rowCount*columnCount), rows(rowCount), columns(columnCount) 
    {
      items = (double*)malloc(itemCount * sizeof(double));
    }

    ~Matrix()
    {
      free(items);
    }

    size_t rowCount() const
    {
      return this->rows;
    }

    size_t columnCount() const
    {
      return this->columns;
    }

    Matrix& operator=(const Matrix &rhs);
    double& operator()(size_t i, size_t j) const;
    Matrix  operator+(const Matrix &rhs);
    Matrix  operator*(const Matrix &rhs);

    double  det(void);
    Matrix  cof(size_t row, size_t column);
    Matrix  adj(void);
    Matrix  inv(void);
    Matrix  T(void);
    Matrix  sqrt(void);
};

#if defined(PCSIM)
std::ostream& operator<<(std::ostream &os, const Matrix &m);
#endif

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* MATRIX_H */

/****************************** END OF FILE **********************************/
