#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactoryCreator.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkBasic.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkMinDist.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactory.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactoryBasic.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactoryMinDist.h>
#include <QtGlobal>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE


//----------
GjkComponentFactory* GjkComponentFactoryCreator::getGjkComponentFactory(const GjkComponentFactoryCreator::GjkType& gjkType)
{
    GjkComponentFactory* p_gjkFactory{nullptr};

    switch(gjkType)
    {
    case GjkType::Basic:
        p_gjkFactory = new GjkComponentFactoryBasic;
        break;
    case GjkType::MinDist:
        p_gjkFactory = new GjkComponentFactoryMinDist;
        break;
    default:
    {
        qFatal("[GjkFactoryCreator::getGjk] unhandled input GjkType enum type!");
        Q_ASSERT(0);
    }
    } //switch
    return(p_gjkFactory);
}

//----------
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
