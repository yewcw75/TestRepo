#ifndef RRTPLANNER_LIB_SIMPLEXQTESTS_H
#define RRTPLANNER_LIB_SIMPLEXQTESTS_H

#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <QObject>
#include <QScopedPointer>

using namespace rrtplanner::framework;
using namespace rrtplanner::framework::algorithm::gjk;

class SimplexQTests : public QObject
{
    Q_OBJECT

public:
    SimplexQTests();
    ~SimplexQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_update_data();
    void verify_update();

private:
    QScopedPointer<Simplex> mp_simplex;
};

#endif
