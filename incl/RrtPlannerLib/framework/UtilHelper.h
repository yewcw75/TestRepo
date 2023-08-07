/**
 * @file UtilHelper.h
 * Helper class containing various utility functions.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 20230703
 */
#ifndef UTILHELPER_H
#define UTILHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class EllMap;
class SPlan;
class SMap;
class RRTPLANNER_LIB_EXPORT UtilHelper
{
public:
    UtilHelper();
    ~UtilHelper();
    /**
     * @brief compare Compares two double values and return true if their difference is smaller than the specified tolerance.
     * @param d1 first double input to compare
     * @param d2 second double input to compare
     * @param tol_small tolerance level
     * @return bool True => difference between d1 and d2 are smaller or equal to tol_small.
     */
    static bool compare(double d1, double d2, double tol_small = 1e-3);

    /**
     * @brief compare Compares two EllMaps and return true if same.
     * @param d1 first EllMap input to compare
     * @param d2 second EllMap input to compare
     * @param tol_small tolerance level for check if float values are same
     * @return bool True => same.
     */
    static bool compare(const EllMap& d1, const EllMap& d2, double tol_small = 1e-3);

    /**
     * @brief compare Compares two SPlans and return true if same.
     * @param d1 first SPlan input to compare
     * @param d2 second SPlan input to compare
     * @param tol_small tolerance level for check if float values are same
     * @return bool True => same.
     */
    static bool compare(const SPlan& d1, const SPlan& d2, double tol_small = 1e-3);

    /**
     * @brief compare Compares two SMaps and return true if same.
     * @param d1 first SMap input to compare
     * @param d2 second SMap input to compare
     * @param tol_small tolerance level for check if float values are same
     * @return bool True => same.
     */
    static bool compare(const SMap& d1, const SMap& d2, double tol_small = 1e-3);

    /**
     * @brief Modulus after division.
     * Equivalent to matlab's mod(x, N).
     * Will always return a value within [0, N) even if x is negative.
     * @param x input integer
     * @param N is the divisor
     * @return results of mod(x, N)
     */
    static int mod(int x, int N);
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif // UTILHELPER_H
