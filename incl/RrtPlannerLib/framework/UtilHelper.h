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
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif // UTILHELPER_H
