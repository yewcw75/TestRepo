#ifndef RRTPLANNER_LIB_PLANQTESTS_H
#define RRTPLANNER_LIB_PLANQTESTS_H

#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <QObject>
#include <QScopedPointer>

using namespace rrtplanner::framework;
class PlanQTests : public QObject
{
    Q_OBJECT

public:
    PlanQTests();
    ~PlanQTests();

private:
    void setup();
    void cleanUp();
    void verifySame(const Waypt& waypt1, const Waypt& waypt2);

private slots:
    void verify_set_plan_data();
    void verify_set_plan();
};

#endif
