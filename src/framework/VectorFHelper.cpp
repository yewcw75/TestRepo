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
