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
  for (size_t i = 0; i < f_sigmas.rows; ++i) {
    Vector si       = sigmapoints.row(i);
    Matrix f_si     = fx(si, dt);
    f_sigmas.row(i) = Vector(f_si);
  }

  // Do the unscented transformation to calculate prior.
  transform(x_prior, P_prior, f_sigmas, Q);
}

void UnscentedKalmanFilter::update(Matrix &z)
{
  // Calculate measurement sigmas by passing prior sigmas through the
  // measurement function
  for (size_t i = 0; i < h_sigmas.rows; ++i) {
    Vector si       = f_sigmas.row(i);
    Matrix h_si     = hx(si);
    h_sigmas.row(i) = Vector(h_si);
  }

  // Calculate the mean and covariance of these points using unscented
  // transformation
  Matrix z_mean(dim_z, 1);
  transform(z_mean, S, h_sigmas, R);

  // Compute cross variance of state x and measurement z.
  Matrix Pxz(dim_x, dim_z);
  Pxz.fill(0.0);
  for (size_t i = 0; i < f_sigmas.rows; ++i) {
    Matrix dx    = f_sigmas.row(i) - x_prior;
    Matrix dz    = h_sigmas.row(i) - z_mean;
    Matrix outer = Vector(dx).outer(dz);
    Matrix Pxzi  = outer * sigmas.Wc(i);
    Pxz          = Pxz + Pxzi;
  }

  // Calculate Kalman gain and residul
  K        = Pxz * S.inv();
  Matrix y = z - z_mean;

  x = x_prior + (K * y);
  P = P_prior - (K * (S * K.T()));
}

void UnscentedKalmanFilter::transform(Matrix &state,
                                      Matrix &covariance,
                                      Matrix &spoints,
                                      Matrix &noise)
{
  // The mean is the dot product of the sigma points and the weight.
  for (size_t i = 0; i < spoints.columns; ++i) {
    state(i) = spoints.column(i).dot(sigmas.Wm);
  }

  // Calculate the state covariance matrix
  covariance.fill(0.0);
  for (size_t i = 0; i < spoints.rows; ++i) {
    Matrix y     = spoints.row(i) - state;
    Matrix outer = Vector(y).outer(y);
    Matrix Pi    = outer * sigmas.Wc(i);
    covariance   = covariance + Pi;
  }

  // Add noise
  covariance = covariance + noise;
}

/****************************** END OF FILE **********************************/
