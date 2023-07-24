#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactoryBasic.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/SimplexBasic.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/SupportBasic.h>
#include <QtGlobal>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

//----------
GjkComponentFactoryBasic::GjkComponentFactoryBasic()
    :GjkComponentFactory()
{

}

//----------
GjkComponentFactoryBasic::~GjkComponentFactoryBasic()
{

}

//----------
Simplex* GjkComponentFactoryBasic::getSimplex(double eps_square)
{
    return(new SimplexBasic(eps_square));
}

//----------
Support* GjkComponentFactoryBasic::getSupport(double eps_square)
{
    return(new SupportBasic(eps_square));
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
