#ifndef RRTPLANNER_LIB_SMAPQTESTS_H
#define RRTPLANNER_LIB_SMAPQTESTS_H

#include <QObject>
#include <QScopedPointer>

class SMapQTests : public QObject
{
    Q_OBJECT

public:
    SMapQTests();
    ~SMapQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_setEllMap_data();
    void verify_setEllMap();
    void verify_reset_data();
    void verify_reset();
};

#endif
