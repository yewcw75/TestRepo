/**
 * @file UblasHelper.h
 * Some functions related to linear algebra using boost::numeric::ublas structures.
 */
#ifndef RRTPLANNER_LIB_UBLASHELPER_H
#define RRTPLANNER_LIB_UBLASHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace bnu = boost::numeric::ublas;

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class RRTPLANNER_LIB_EXPORT UblasHelper
{
public:
    UblasHelper();
    ~UblasHelper();

    static bnu::matrix<double> concatenate_col_vectors(const bnu::vector<double>& v1,
                                                      const bnu::vector<double>& v2
                                                      ); //treat v1 and v2 as col vectors, and concatenate column-wise. i.e. [v1, v2]

    static bool solve(const bnu::matrix<double>& M, //invertible square matrix, [NxN]
                      const bnu::vector<double>& y, //[Nx1]
                      double tol_small, //for checking if ok to 1/det(M)
                      bnu::vector<double>& x //[Nx1]
                      );

};


RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
