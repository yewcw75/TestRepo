/**
 * @file GeometryHelper.h
 * Helper class for geometry structures, Coord_NE, Vector_NE, etc, that are defined in FrameworkDefines.h
 *
 * Functions here abstract away the underlying library operations for Coord_NE and Vector_NE.
 * Makes refactoring easier if need to change the underlying data structures of Coord_NE and Vector_NE in future.
 *
 * In addition, many boost geometry functions, e.g. multiply_value, dot_product, etc, perform in-place operation on the
 * input data. Here functions are written to return a new data object while keeping the input data constant.
 */
#ifndef RRTPLANNER_LIB_GEOMETRY_HELPER_H
#define RRTPLANNER_LIB_GEOMETRY_HELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class RRTPLANNER_LIB_EXPORT GeometryHelper
{
public:
    GeometryHelper();
    ~GeometryHelper();

    static float norm2(const Vector_NE& vec); //sqrt(vec.' * vec)
    static Vector_NE multiply_value(const Vector_NE& vec, float scalar); //scalar * vec
    static Vector_NE add_vector(const Vector_NE& vec1, const Vector_NE& vec2); //vec1 + vec2
    static Vector_NE subtract_vector(const Vector_NE& vec1, const Vector_NE& vec2); //vec1 - vec2
    static float dot_product(const Vector_NE& vec1, const Vector_NE& vec2); //vec1 dot vec2)

    static Vector_NE subtract_point(const Coord_NE& pt1, const Coord_NE& pt2); //pt1 - pt2
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif
