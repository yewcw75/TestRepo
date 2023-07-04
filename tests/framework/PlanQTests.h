#ifndef RRTPLANNER_LIB_PLANQTESTS_H
#define RRTPLANNER_LIB_PLANQTESTS_H

#include <RrtPlannerLib/framework/Waypt.h>
#include <QObject>
#include <QScopedPointer>

class PlanQTests : public QObject
{
    Q_OBJECT

public:
    PlanQTests();
    ~PlanQTests();

private:
    void setup();
    void cleanUp();
    void verifySame(const rrtplanner::framework::Waypt& waypt1, const rrtplanner::framework::Waypt& waypt2);

private slots:
    void verify_set_plan_data();
    void verify_set_plan();
};

#endif
