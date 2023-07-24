#include <RrtPlannerLib/framework/algorithm/gjk/GjkFactory.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkBasic.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkMinDist.h>
#include <QtGlobal>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

//----------
Gjk* GjkFactory::getGjk(const  GjkFactory::GjkType& gjkType)
{
    Gjk* p_gjk{nullptr};

    switch(gjkType)
    {
    case GjkType::Basic:
        p_gjk = new GjkBasic;
        break;
    case GjkType::MinDist:
        p_gjk = new GjkMinDist;
        break;
    default:
    {
        qFatal("[GjkFactoryCreator::getGjk] unhandled input GjkType enum type!");
        Q_ASSERT(0);
    }
    } //switch
    return(p_gjk);
}

//----------
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
