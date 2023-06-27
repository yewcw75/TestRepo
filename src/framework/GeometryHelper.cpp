#include <RrtPlannerLib/framework/GeometryHelper.h>
#include <cmath>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//----------
GeometryHelper::GeometryHelper()
{

}

//----------
GeometryHelper::~GeometryHelper()
{

}

//----------
float GeometryHelper::lengthVector(const Vector_NE& vec)
{
    return(std::sqrt(bg::dot_product(vec, vec)));
}

//----------
Vector_NE GeometryHelper::scalar_multiplyVector(float scalar, const Vector_NE& vec)
{
    Vector_NE res = vec;
    bg::multiply_value(res, scalar);
    return(res);
}

//----------
Vector_NE GeometryHelper::addVector(const Vector_NE& vec1, const Vector_NE& vec2)
{
    Vector_NE res = vec1;
    bg::add_point(res, vec2);
    return(res);
}

//----------
Vector_NE GeometryHelper::subtractVector(const Vector_NE& vec1, const Vector_NE& vec2)
{
    Vector_NE res = vec1;
    bg::subtract_point(res, vec2);
    return(res);
}

//----------
float GeometryHelper::dotProductVector(const Vector_NE& vec1, const Vector_NE& vec2)
{
    return(bg::dot_product(vec1, vec2));
}

//----------
Vector_NE GeometryHelper::subtractPoint(const Coord_NE& pt1, const Coord_NE& pt2)
{
    Vector_NE res = pt1;
    bg::subtract_point(res, pt2);
    return(res);
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
