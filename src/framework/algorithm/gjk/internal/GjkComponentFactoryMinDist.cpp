#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactoryMinDist.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/SimplexMinDist.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/SupportMinDist.h>
#include <QtGlobal>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

//----------
GjkComponentFactoryMinDist::GjkComponentFactoryMinDist()
    :GjkComponentFactory()
{

}

//----------
GjkComponentFactoryMinDist::~GjkComponentFactoryMinDist()
{

}

//----------
Simplex* GjkComponentFactoryMinDist::getSimplex(double eps_square)
{
    return(new SimplexMinDist(eps_square));
}

//----------
Support* GjkComponentFactoryMinDist::getSupport(double eps_square)
{
    return(new SupportMinDist(eps_square));
}
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
