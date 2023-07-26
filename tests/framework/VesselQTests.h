#ifndef RRTPLANNER_LIB_VESSELQTESTS_H
#define RRTPLANNER_LIB_VESSELQTESTS_H

#include <RrtPlannerLib/framework/VesRectangle.h>
#include <QObject>
#include <QScopedPointer>

class VesselQTests : public QObject
{
    Q_OBJECT

public:
    VesselQTests();
    ~VesselQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_constructor_data();
    void verify_constructor();
};

#endif
