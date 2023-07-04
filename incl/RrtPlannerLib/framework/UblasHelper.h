/**
 * @file UblasHelper.h
 * @brief Contains functions related to linear algebra using boost::numeric::ublas structures.
 */

#ifndef RRTPLANNER_LIB_UBLASHELPER_H
#define RRTPLANNER_LIB_UBLASHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace bnu = boost::numeric::ublas;

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

/**
 * @class UblasHelper
 * @brief Provides helper functions for linear algebra operations using boost::numeric::ublas structures.
 */
class RRTPLANNER_LIB_EXPORT UblasHelper
{
public:
    /**
     * @brief Default constructor.
     */
    UblasHelper();

    /**
     * @brief Destructor.
     */
    ~UblasHelper();

    /**
     * @brief Concatenates two column vectors column-wise.
     * @param v1 The first column vector.
     * @param v2 The second column vector.
     * @return The concatenated matrix of v1 and v2.
     */
    static bnu::matrix<double> concatenate_col_vectors(const bnu::vector<double>& v1,
                                                       const bnu::vector<double>& v2);

    /**
     * @brief Solves a linear system of equations.
     * @param M The invertible square matrix of size NxN.
     * @param y The column vector of size Nx1.
     * @param tol_small The tolerance value used to check if it is safe to calculate 1/det(M).
     * @param x The solution vector of size Nx1.
     * @return True if the system is solved successfully, false otherwise.
     */
    static bool solve(const bnu::matrix<double>& M,
                      const bnu::vector<double>& y,
                      double tol_small,
                      bnu::vector<double>& x);
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
