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
  Matrix f_sigmapoints = Matrix(sigmas.numOfSigmas(), dim_x);
  f_sigmapoints.fill(0.0);

  for(size_t i = 0; i < sigmas.numOfSigmas(); ++i) {
    Vector si = sigmapoints.row(i);
    Matrix f_si = fx(si, dt);
    f_sigmapoints.row(i) = Vector(f_si);
  }

  // Unscented transformation
  // The mean is the dot product of the sigma points and the weight.
  for(size_t i = 0; i < dim_x; ++i) {
    x_prior(i) = f_sigmapoints.column(i).dot(sigmas.Wm);
  }

  // Calculate the state covariance matrix
  P_prior.fill(0.0);
  for(size_t i = 0; i < f_sigmapoints.rows; ++i) {
    Matrix y = f_sigmapoints.row(i) - x_prior;
    Matrix outer = Vector(y).outer(y);
    Matrix Pi = outer * sigmas.Wc(i);
    P_prior = P_prior + Pi;
  }

  // Add process noise
  P_prior = P_prior + Q;
}

void UnscentedKalmanFilter::update(Matrix &z)
{
}
/****************************** END OF FILE **********************************/
