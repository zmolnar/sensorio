/**
 * @file ukf.h
 * @brief
 */

#ifndef UKF_H
#define UKF_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "SigmaPoints.h"
#include "Matrix.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef Matrix (*fx_t)(const Vector &x, double dt);
typedef Matrix (*hx_t)(const Vector &x);

class UnscentedKalmanFilter {
public:
  const size_t dim_x;
  const size_t dim_z;

  Matrix x;       // State vector
  Matrix P;       // Process state covariance matrix
  Matrix x_prior; // State vector
  Matrix P_prior; // Process state covariance matrix
  Matrix Q;       // Process noise
  Matrix R;       // Measurement covariance matrix

  Matrix K;       // Kalman gain
  Matrix S;       // System uncertainty

  Matrix f_sigmas;
  Matrix h_sigmas;
  const fx_t   fx;
  const hx_t   hx;
  SigmaPoints &sigmas;
  
  UnscentedKalmanFilter(size_t       dim_x,
                        size_t       dim_z,
                        fx_t         fx,
                        hx_t         hx,
                        SigmaPoints &sigmas) :
      dim_x(dim_x),
      dim_z(dim_z),
      x(dim_x, 1),
      P(dim_x, dim_x),
      x_prior(dim_x, 1),
      P_prior(dim_x, dim_x),
      Q(dim_x, dim_x),
      R(dim_z, dim_z),
      K(dim_x, dim_z),
      S(dim_z, dim_z),
      f_sigmas(sigmas.numOfSigmas(), dim_x),
      h_sigmas(sigmas.numOfSigmas(), dim_z),
      fx(fx),
      hx(hx),
      sigmas(sigmas)
  {
  }

  ~UnscentedKalmanFilter(){};

  void predict(double dt);
  void update(Matrix &z);
  void transform(Matrix &x, Matrix &P, Matrix &s, Matrix &noise);
};

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* UKF_H */

/****************************** END OF FILE **********************************/
