#ifndef RRTPLANNER_LIB_WAYPTQTESTS_H
#define RRTPLANNER_LIB_WAYPTQTESTS_H

#include <RrtPlannerLib/framework/Waypt.h>
#include <QObject>
#include <QScopedPointer>

using namespace rrtplanner::framework;
class WayptQTests : public QObject
{
    Q_OBJECT

public:
    WayptQTests();
    ~WayptQTests();

private:
    void setup();
    void cleanUp();
    void verify(Waypt waypt, float northing, float easting, double lon0, int id);

private slots:
    void verify_set_params_data();
    void verify_set_params();
    void verify_setters_data();
    void verify_setters();
    void verify_copy_data();
    void verify_copy();
};

#endif
