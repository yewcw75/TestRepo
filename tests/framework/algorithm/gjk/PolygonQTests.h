#ifndef RRTPLANNER_LIB_POLYGONQTESTS_H
#define RRTPLANNER_LIB_POLYGONQTESTS_H

#include <QObject>
#include <QScopedPointer>

class PolygonQTests : public QObject
{
    Q_OBJECT

public:
    PolygonQTests();
    ~PolygonQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_support_data();
    void verify_support();
    void verify_clone_data();
    void verify_clone();
    void verify_centroid_data();
    void verify_centroid();
};

#endif
