#include <RrtPlannerLib/framework/algorithm/gjk/internal/SupportMinDist.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

//function aliases for readability here
static const auto& dot = VectorFHelper::dot_product;
static const auto& add = VectorFHelper::add_vector;
//----------
SupportMinDist::SupportMinDist()
    :Support()
{

}

//----------
SupportMinDist::SupportMinDist(double eps_square)
    :Support(eps_square)
{

}

//----------
SupportMinDist::~SupportMinDist()
{

}

//----------
void SupportMinDist::reset()
{

}

//----------
SupportMinDist::ResultFlag SupportMinDist::support(const IShape* shape1, const IShape* shape2,
                            const VectorF& v, bool isDirFromSupport2Origin,
                            VectorF& spp)
{
    VectorF spp1 = shape1->support(VectorFHelper::multiply_value(v, -1.0));
    VectorF spp2 = shape2->support(v);
    spp = VectorFHelper::subtract_vector(spp2, spp1);

    //check if support is at origin
    Support::ResultFlag resultFlag{Support::ResultFlag::SUPPORT_BEYOND_ORIGIN}; //spp beyond origin
    if( dot(spp, spp) < eps_square()) { //case 1. equivalent to |w| < eps
        resultFlag = Support::ResultFlag::SUPPORT_ON_ORIGIN; //spp at origin
    }
    else if(isDirFromSupport2Origin){
        double s = dot(add(spp, v), v);
        double v_dot_v = dot(v, v);
        if(s*s < eps_square()*v_dot_v) {// case 2. equivalent to s/|v| < eps
            resultFlag = Support::ResultFlag::SUPPORT_ON_SIMPLEX; //spp misses origin => both polygons do not intersect
        }
    }
    return(resultFlag);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
