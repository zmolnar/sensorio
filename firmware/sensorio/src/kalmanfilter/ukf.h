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
#include "matrix.h"

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
  const double dt;

  Matrix x;       // State vector
  Matrix P;       // Process state covariance matrix
  Matrix x_prior; // State vector
  Matrix P_prior; // Process state covariance matrix
  Matrix Q;       // Process noise
  Matrix R;       // Measurement covariance matrix

  UnscentedKalmanFilter(size_t       dim_x,
                        size_t       dim_z,
                        double       dt,
                        fx_t         fx,
                        hx_t         hx,
                        SigmaPoints &sigmas) :
      dim_x(dim_x),
      dim_z(dim_z),
      dt(dt),
      x(Matrix(dim_x, 1)),
      P(Matrix(dim_x, dim_x)),
      x_prior(Matrix(dim_x, 1)),
      P_prior(Matrix(dim_x, dim_x)),
      Q(Matrix(dim_x, dim_x)),
      R(Matrix(dim_z, dim_z)),
      fx(fx),
      hx(hx),
      sigmas(sigmas)
  {
  }

  ~UnscentedKalmanFilter(){};

  void predict(void);
  void update(Matrix &z);

private:
  const fx_t   fx;
  const hx_t   hx;
  SigmaPoints &sigmas;

  
};

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* UKF_H */

/****************************** END OF FILE **********************************/