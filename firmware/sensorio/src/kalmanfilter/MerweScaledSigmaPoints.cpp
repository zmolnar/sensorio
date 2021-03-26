/**
 * @file MerweScaledSiagmaPoints.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "MerweScaledSigmaPoints.h"
#include "env.h"

#include <cassert>

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
size_t MerweScaledSigmaPoints::numOfSigmas(void)
{
  return 2 * n + 1;
}

Matrix MerweScaledSigmaPoints::generateSigmas(Matrix &x, Matrix &P)
{
  // X must be a matrix(n, 1)
  ASSERT(n == x.rows);
  ASSERT(1 == x.columns);

  // P must be an n dimensional square matrix
  ASSERT(n == P.rows);
  ASSERT(n == P.columns);

  Matrix U = ((lambda + n) * P).sqrt();

  // Create sigma points matrix whose rows contain the generated points.
  Matrix sigmas = Matrix(this->numOfSigmas(), n);
  sigmas.fill(0.0);

  // The first row is the mean
  for (size_t i = 0; i < n; ++i) {
    sigmas(0, i) = x(i);
  }

  for (size_t k = 0; k < n; ++k) {
    // Xi_1..n
    for (size_t i = 0; i < n; ++i) {
      sigmas(k + 1, i) = x(i) + U(i, k);
    }

    // Xi_n+1..2n
    for (size_t i = 0; i < n; ++i) {
      sigmas(n + k + 1, i) = x(i) - U(i, k);
    }
  }

  return sigmas;
}
#include <iostream>
void MerweScaledSigmaPoints::computeWeights(void)
{
  // Initialize weight matrix
  double Wi = 0.5 / (n + lambda);
  Wm.fill(Wi);
  Wc.fill(Wi);

  // Set weight for the mean[0]
  Wm(0, 0) = lambda / (n + lambda); 

  // Set weight for the covariance[0]
  Wc(0, 0) = (lambda / (n + lambda)) + (1 - (alpha * alpha) + beta); 
}

/****************************** END OF FILE **********************************/
