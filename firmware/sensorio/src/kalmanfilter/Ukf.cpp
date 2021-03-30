/**
 * @file ukf.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Ukf.h"
#include "env.h"

#include <iostream>

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
void UnscentedKalmanFilter::predict(void)
{
  // Generate sigma points
  Matrix sigmapoints = sigmas.generateSigmas(x, P);

  // Compute the prior using the state transition function
  f_sigmas = Matrix(sigmas.numOfSigmas(), dim_x);
  f_sigmas.fill(0.0);

  for(size_t i = 0; i < sigmas.numOfSigmas(); ++i) {
    Vector si = sigmapoints.row(i);
    Matrix f_si = fx(si, dt);
    f_sigmas.row(i) = Vector(f_si);
  }

  // Do the unscented transformation to calculate prior.
  transform(x_prior, P_prior, f_sigmas, Q);
}

void UnscentedKalmanFilter::update(Matrix &z)
{
}

void UnscentedKalmanFilter::transform(Matrix &x, Matrix &P, Matrix &s, Matrix &noise)
{
  // The mean is the dot product of the sigma points and the weight.
  for(size_t i = 0; i < s.columns; ++i) {
    x(i) = s.column(i).dot(sigmas.Wm);
  }

  // Calculate the state covariance matrix
  P.fill(0.0);
  for(size_t i = 0; i < s.rows; ++i) {
    Matrix y = s.row(i) - x;
    Matrix outer = Vector(y).outer(y);
    Matrix Pi = outer * sigmas.Wc(i);
    P = P + Pi;
  }

  // Add process noise
  P = P + noise;
}
/****************************** END OF FILE **********************************/
