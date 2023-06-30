#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <cmath>
#include <boost/numeric/ublas/vector.hpp>
//#include <boost/geometry.hpp>

//namespace bg = boost::geometry;
namespace bnu = boost::numeric::ublas;

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//----------
VectorFHelper::VectorFHelper()
{

}

//----------
VectorFHelper::~VectorFHelper()
{

}

//----------
float VectorFHelper::norm2(const VectorF& vec)
{
    //return(std::sqrt(bg::dot_product(vec, vec)));
    return(bnu::norm_2(vec.data_const_ref()));
}

//----------
VectorF VectorFHelper::multiply_value(const VectorF& vec, float scalar)
{
    /*
    Vector_NE res = vec;
    bg::multiply_value(res, scalar);
    return(res);
    */
    return(VectorF(scalar * vec.data_const_ref()));
}

//----------
VectorF VectorFHelper::add_vector(const VectorF& vec1, const VectorF& vec2)
{
    /*
    Vector_NE res = vec1;
    bg::add_point(res, vec2);
    return(res);
    */
    return(VectorF(vec1.data_const_ref() + vec2.data_const_ref()));
}

//----------
VectorF VectorFHelper::subtract_vector(const VectorF& vec1, const VectorF& vec2)
{
    /*
    Vector_NE res = vec1;
    bg::subtract_point(res, vec2);
    return(res);
    */
    return(VectorF(vec1.data_const_ref() - vec2.data_const_ref()));
}

//----------
float VectorFHelper::dot_product(const VectorF& vec1, const VectorF& vec2)
{
    //return(bg::dot_product(vec1, vec2));
    return(bnu::inner_prod(vec1.data_const_ref(), vec2.data_const_ref()));
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
