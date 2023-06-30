#ifndef RRTPLANNER_LIB_PLANHELPERQTESTS_H
#define RRTPLANNER_LIB_PLANHELPERQTESTS_H

#include <RrtPlannerLib/framework/Plan.h>
#include <QObject>
#include <QScopedPointer>

using namespace rrtplanner::framework;
class PlanHelperQTests : public QObject
{
    Q_OBJECT

public:
    PlanHelperQTests();
    ~PlanHelperQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_findNearestEdgeEventdata_data();
    void verify_findNearestEdgeEventdata();
};

#endif
