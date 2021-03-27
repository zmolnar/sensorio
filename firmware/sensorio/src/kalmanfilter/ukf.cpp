/**
 * @file ukf.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ukf.h"
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

    std::cout << "i" << std::endl << i << std::endl;
    std::cout << "y" << std::endl << y << std::endl;
    std::cout << "outer" << std::endl << outer << std::endl;
    
    Matrix Pi = outer * sigmas.Wc(i); 
    std::cout << "Pi" << i << std::endl << Pi << std::endl;
    P_prior = P_prior + Pi;

    std::cout << "P" << i << std::endl << P_prior << std::endl;
  }

  std::cout << "P_prior" << std::endl << P_prior << std::endl;

  // Add process noise
  P_prior = P_prior + Q;

  std::cout << "x" << std::endl << x << std::endl;
  std::cout << "P" << std::endl << P << std::endl;
  std::cout << "Q" << std::endl << Q << std::endl;
  std::cout << "Wm" << std::endl << sigmas.Wm << std::endl;
  std::cout << "Wc" << std::endl << sigmas.Wc << std::endl;
  std::cout << "Sigmapoints" << std::endl << sigmapoints << std::endl;
  std::cout << "f_Sigmapoints" << std::endl << f_sigmapoints << std::endl;
  std::cout << "x_prior" << std::endl << x_prior << std::endl;
  std::cout << "P_prior" << std::endl << P_prior << std::endl;
}

void UnscentedKalmanFilter::update(Matrix &z)
{
}
/****************************** END OF FILE **********************************/
