#ifndef RRTPLANNER_LIB_ROOTDATAQTESTS_H
#define RRTPLANNER_LIB_ROOTDATAQTESTS_H

#include <QObject>
#include <QScopedPointer>

class RootDataQTests : public QObject
{
    Q_OBJECT

public:
    RootDataQTests();
    ~RootDataQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_constructor();
    void verify_setters_getters();
};

#endif
