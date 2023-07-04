#ifndef RRTPLANNER_LIB_PLANHELPERQTESTS_H
#define RRTPLANNER_LIB_PLANHELPERQTESTS_H

#include <QObject>
#include <QScopedPointer>

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
    void verify_getCrossTrackPlan_data();
    void verify_getCrossTrackPlan();
    void verify_pushPlan();
    void verify_buildSingleSideEllMaps_data();
    void verify_buildSingleSideEllMaps();
};

#endif
