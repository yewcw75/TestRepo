/**
 * @file LinearAlgebraHelper.h
 * Some functions related to linear algebra.
 */
#ifndef RRTPLANNER_LIB_LINEARALGEBRAHELPER_H
#define RRTPLANNER_LIB_LINEARALGEBRAHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class RRTPLANNER_LIB_EXPORT LinearAlgebraHelper
{
public:
    LinearAlgebraHelper();
    ~LinearAlgebraHelper();

    static bool solve(const boost::numeric::ublas::matrix<float>& M, //invertible square matrix, [NxN]
                      const boost::numeric::ublas::vector<float>& y, //[Nx1]
                      float tol_small, //for checking if ok to 1/det(M)
                      boost::numeric::ublas::vector<float>& x //[Nx1]
                      );

};


RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
