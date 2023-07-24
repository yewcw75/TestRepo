#ifndef RRTPLANNER_LIB_GJKQTESTS_H
#define RRTPLANNER_LIB_GJKQTESTS_H

#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <QObject>
#include <QScopedPointer>

using namespace rrtplanner::framework;
using namespace rrtplanner::framework::algorithm::gjk;

class GjkQTests : public QObject
{
    Q_OBJECT

public:
    GjkQTests();
    ~GjkQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_chkIntersect_data();
    void verify_chkIntersect();

private:
    QScopedPointer<Gjk> mp_gjk;
    QScopedPointer<Gjk> mp_gjkMinDist;
};

#endif
