#ifndef RRTPLANNER_LIB_WAYPTQTESTS_H
#define RRTPLANNER_LIB_WAYPTQTESTS_H

#include <RrtPlannerLib/framework/Waypt.h>
#include <QObject>
#include <QScopedPointer>

class WayptQTests : public QObject
{
    Q_OBJECT

public:
    WayptQTests();
    ~WayptQTests();

private:
    void setup();
    void cleanUp();
    void verify(rrtplanner::framework::Waypt waypt, double northing, double easting, double lon0, int id);

private slots:
    void verify_set_params_data();
    void verify_set_params();
    void verify_setters_data();
    void verify_setters();
    void verify_copy_data();
    void verify_copy();
};

#endif
