#ifndef RRTPLANNER_LIB_SIMPLEXQTESTS_H
#define RRTPLANNER_LIB_SIMPLEXQTESTS_H

#include <QObject>

class SimplexQTests : public QObject
{
    Q_OBJECT

public:
    SimplexQTests();
    ~SimplexQTests();

private:
    void setup();
    void cleanUp();

private slots:
    void verify_update_data();
    void verify_update();
};

#endif
