/**
 * @file GeometryHelper.h
 * Helper class for geometry structures, Coord_NE, Vector_NE, etc, that are defined in FrameworkDefines.h
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

    static float lengthVector(const Vector_NE& vec);
    static Vector_NE scalar_multiplyVector(float scalar, const Vector_NE& vec);
    static Vector_NE addVector(const Vector_NE& vec1, const Vector_NE& vec2); //vec1 + vec2
    static Vector_NE subtractVector(const Vector_NE& vec1, const Vector_NE& vec2); //vec1 - vec2
    static float dotProductVector(const Vector_NE& vec1, const Vector_NE& vec2); //vec1 dot vec2)

    static Vector_NE subtractPoint(const Coord_NE& pt1, const Coord_NE& pt2); //pt1 - pt2
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif
