#include <RrtPlannerLib/framework/algorithm/gjk/internal/SupportBasic.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

//function aliases for readability here
static const auto& dot = VectorFHelper::dot_product;

//----------
SupportBasic::SupportBasic()
    :Support()
{

}

//----------
SupportBasic::SupportBasic(double eps_square)
    :Support(eps_square)
{

}

//----------
SupportBasic::~SupportBasic()
{

}

//----------
void SupportBasic::reset()
{

}

//----------
SupportBasic::ResultFlag SupportBasic::support(const IShape* shape1, const IShape* shape2,
                            const VectorF& v, bool isDirFromSupport2Origin,
                            VectorF& spp)
{
    VectorF spp1 = shape1->support(VectorFHelper::multiply_value(v, -1.0));
    VectorF spp2 = shape2->support(v);
    spp = VectorFHelper::subtract_vector(spp2, spp1);

    //check if support is at origin
    Support::ResultFlag resultFlag{Support::ResultFlag::SUPPORT_BEYOND_ORIGIN}; //spp beyond origin
    if( dot(spp, spp) < eps_square()) { //equivalent to |spp| < sqrt(eps_rel)
        resultFlag = Support::ResultFlag::SUPPORT_ON_ORIGIN; //spp at origin
    }
    else{
        double spp_dot_v = dot(spp, v);
        double v_dot_v = dot(v, v);
        if(spp_dot_v < 0 && spp_dot_v*spp_dot_v > eps_square()*v_dot_v) {// equivalent to (spp.v)/|v| < -sqrt(eps_rel)
            resultFlag = Support::ResultFlag::SUPPORT_SHORT_OF_ORIGIN; //spp misses origin => both polygons do not intersect
        }
    }
    return(resultFlag);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
