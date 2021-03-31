/**
 * @file MerweScaledSigmaPoints.h
 * @brief
 */

#ifndef MERWE_SCALED_SIAGMA_POINTS_H
#define MERWE_SCALED_SIAGMA_POINTS_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "SigmaPoints.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
class MerweScaledSigmaPoints : public SigmaPoints {
public:
  MerweScaledSigmaPoints(size_t n, double a, double b, double k) :
      SigmaPoints(n, 2 * n + 1),
      alpha(a),
      beta(b),
      kappa(k),
      lambda(alpha * alpha * (n + kappa) - n)
  {
    computeWeights();
  }
  ~MerweScaledSigmaPoints()
  {
  }

  size_t numOfSigmas(void);
  Matrix generateSigmas(Matrix &x, Matrix &P);
  void   computeWeights(void);

private:
  const double alpha;
  const double beta;
  const double kappa;
  const double lambda;
};

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* MERWE_SCALED_SIAGMA_POINTS_H */

/****************************** END OF FILE **********************************/
