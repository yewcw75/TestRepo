#ifndef RRTPLANNER_LIB_VECTORFQTESTS_H
#define RRTPLANNER_LIB_VECTORFQTESTS_H

#include <RrtPlannerLib/framework/VectorF.h>
#include <QObject>

using namespace rrtplanner::framework;
class VectorFQTests : public QObject
{
    Q_OBJECT

public:
    VectorFQTests();
    ~VectorFQTests();

private slots:
    void verify_constructors();
    void verify_copy();
    void verify_resize();
    void verify_access();
};

#endif
