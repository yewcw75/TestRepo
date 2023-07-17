#ifndef RRTPLANNER_LIB_GJKQTESTS_H
#define RRTPLANNER_LIB_GJKQTESTS_H

#include <QObject>

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
};

#endif
