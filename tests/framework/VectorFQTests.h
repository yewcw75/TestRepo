#ifndef RRTPLANNER_LIB_VECTORFQTESTS_H
#define RRTPLANNER_LIB_VECTORFQTESTS_H

#include <QObject>

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
