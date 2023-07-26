#ifndef RRTPLANNER_LIB_VESRECTANGLEQTESTS_H
#define RRTPLANNER_LIB_VESRECTANGLEQTESTS_H

#include <RrtPlannerLib/framework/VesRectangle.h>
#include <QObject>
#include <QScopedPointer>

class VesRectangleQTests : public QObject
{
    Q_OBJECT

public:
    VesRectangleQTests();
    ~VesRectangleQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_constructor();
    void verify_getPolygon();
    void verify_clone();
};

#endif
