#ifndef RRTPLANNER_LIB_ELLMAPQTESTS_H
#define RRTPLANNER_LIB_ELLMAPQTESTS_H

#include <RrtPlannerLib/framework/EllMap.h>
#include <QObject>
#include <QScopedPointer>

class EllMapQTests : public QObject
{
    Q_OBJECT

public:
    EllMapQTests();
    ~EllMapQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_buildEllMap_data();
    void verify_buildEllMap();
};

#endif
