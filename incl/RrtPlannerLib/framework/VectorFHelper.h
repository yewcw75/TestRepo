/**
 * @file VectorFHelper.h
 * @see VectorF.h
 * Helper class for VectorF.
 *
 * Functions here abstract away the underlying library operations for VectorF objects.
 * Makes refactoring easier if need to change the underlying data structures in future.
 *
 */
#ifndef RRTPLANNER_LIB_VECTORFHELPER_H
#define RRTPLANNER_LIB_VECTORFHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class RRTPLANNER_LIB_EXPORT VectorFHelper
{
public:
    VectorFHelper();
    ~VectorFHelper();

    static float norm2(const VectorF& vec); //sqrt(vec.' * vec)
    static VectorF multiply_value(const VectorF& vec, float scalar); //scalar * vec
    static VectorF add_vector(const VectorF& vec1, const VectorF& vec2); //vec1 + vec2
    static VectorF subtract_vector(const VectorF& vec1, const VectorF& vec2); //vec1 - vec2
    static float dot_product(const VectorF& vec1, const VectorF& vec2); //vec1 dot vec2)
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif
