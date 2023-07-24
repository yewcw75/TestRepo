#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <cmath>
#include <boost/numeric/ublas/vector.hpp>

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
double VectorFHelper::norm2(const VectorF& vec)
{
    return(bnu::norm_2(vec.data_const_ref()));
}

//----------
VectorF VectorFHelper::multiply_value(const VectorF& vec, double scalar)
{
    return(VectorF(scalar * vec.data_const_ref()));
}

//----------
VectorF VectorFHelper::add_vector(const VectorF& vec1, const VectorF& vec2)
{
    return(VectorF(vec1.data_const_ref() + vec2.data_const_ref()));
}

//----------
VectorF VectorFHelper::subtract_vector(const VectorF& vec1, const VectorF& vec2)
{
    return(VectorF(vec1.data_const_ref() - vec2.data_const_ref()));
}

//----------
double VectorFHelper::dot_product(const VectorF& vec1, const VectorF& vec2)
{
    return(bnu::inner_prod(vec1.data_const_ref(), vec2.data_const_ref()));
}

//----------
double VectorFHelper::cross_product_zVal(const VectorF& vec1, const VectorF& vec2)
{
    double zVal{0.0};
    const int DIM_EXPECT = 2;

    if(vec1.size() != DIM_EXPECT || vec2.size() != DIM_EXPECT){
        qWarning() << "[VectorFHelper::cross_product_zVal] Either one or both VectorF input are not of expected dimension of 2. Results may be errorneous!";
        Q_ASSERT(0);
    }
    if(vec1.size() >= DIM_EXPECT && vec2.size() >= DIM_EXPECT){
        zVal = vec1[0]*vec2[1] - vec1[1]*vec2[0];
     }
    return(zVal);
}

//----------
VectorF VectorFHelper::cross_product(const VectorF& vec1, const VectorF& vec2)
{
    const int DIM_EXPECT = 3;

    VectorF vecOut{0,0,0};

    if(vec1.size() != DIM_EXPECT || vec2.size() != DIM_EXPECT){
        qWarning() << "[VectorFHelper::cross_product] Either one or both VectorF input are not of expected dimension of 3. Results may be errorneous!";
        Q_ASSERT(0);
    }
    if(vec1.size() >= DIM_EXPECT && vec2.size() >= DIM_EXPECT){
        vecOut[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
        vecOut[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
        vecOut[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
     }
    return(vecOut);
}

//----------
VectorF VectorFHelper::vec2D_cross_z(const VectorF& vec)
{
    VectorF vecOut{0, 0};

    const int DIM_EXPECT = 2;

    if(vec.size() != DIM_EXPECT){
        qWarning() << "[VectorFHelper::vec2D_cross_z] The VectorF input is not of expected dimension of 2. Results may be errorneous!";
        Q_ASSERT(0);
    }
    if(vec.size() >= DIM_EXPECT){
        vecOut[0] = vec[1];
        vecOut[1] = -vec[0];
     }
    return(vecOut);
}

//----------
bool VectorFHelper::compare(const VectorF& vec1, const VectorF& vec2, double tol_small)
{
    Q_ASSERT(tol_small >= 0);
    if(tol_small < 0){
        tol_small = abs(tol_small);
        qWarning() << "[VectorFHelper::compare] input tol_small is negative when it is expected to be positive. The absolute value of it is used instead.";
    }
    VectorF vDiff = VectorFHelper::subtract_vector(vec1, vec2);
    double norm = VectorFHelper::norm2(vDiff);
    return(norm <= tol_small);
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
