#ifndef RRTPLANNER_LIB_SMAPHELPERQTESTS_H
#define RRTPLANNER_LIB_SMAPHELPERQTESTS_H

#include <QObject>
#include <QScopedPointer>

class SMapHelperQTests : public QObject
{
    Q_OBJECT

public:
    SMapHelperQTests();
    ~SMapHelperQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_determineArcLengthHorizon_data();
    void verify_determineArcLengthHorizon();
    void verify_vol3_data();
    void verify_vol3();
    void verify_vol4_data();
    void verify_vol4();
    void verify_create_data();
    void verify_create();
};

#endif
