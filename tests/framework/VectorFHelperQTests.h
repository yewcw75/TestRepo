#ifndef RRTPLANNER_LIB_VECTORFHELPERQTESTS_H
#define RRTPLANNER_LIB_VECTORFHELPERQTESTS_H

#include <QObject>

class VectorFHelperQTests : public QObject
{
    Q_OBJECT

public:
    VectorFHelperQTests();
    ~VectorFHelperQTests();

private slots:
    void verify_cross_product_zVal_data();
    void verify_cross_product_zVal();
    void verify_cross_product_data();
    void verify_cross_product();
    void verify_vec2D_cross_z_data();
    void verify_vec2D_cross_z();
};

#endif
