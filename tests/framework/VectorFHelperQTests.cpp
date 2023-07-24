#include "VectorFHelperQTests.h"
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <QVector>
#include <iostream>

using namespace rrtplanner::framework;
namespace bnu = boost::numeric::ublas;

//----------
VectorFHelperQTests::VectorFHelperQTests()
{

}

//----------
VectorFHelperQTests::~VectorFHelperQTests()
{

}

//----------
void VectorFHelperQTests::verify_cross_product_zVal_data()
{
    QTest::addColumn<VectorF>("vec1");
    QTest::addColumn<VectorF>("vec2");
    QTest::addColumn<double>("zVal_expect");

    VectorF vec1{1, 20};
    VectorF vec2{-20, 99};
    double zVal_expect = 499;


    QTest::newRow("Test 1") << vec1 << vec2 << zVal_expect;
    return;
}

//----------
void VectorFHelperQTests::verify_cross_product_zVal()
{
    QFETCH(VectorF, vec1);
    QFETCH(VectorF, vec2);
    QFETCH(double, zVal_expect);

    double zVal = VectorFHelper::cross_product_zVal(vec1, vec2);
    QVERIFY(UtilHelper::compare(zVal, zVal_expect));
    return;
}

//----------
void VectorFHelperQTests::verify_cross_product_data()
{
    QTest::addColumn<VectorF>("vec1");
    QTest::addColumn<VectorF>("vec2");
    QTest::addColumn<VectorF>("vecOut_expect");

    VectorF vec1{-1.3077, -0.4336, 0.3426};
    VectorF vec2{3.5784, 2.7694, -1.3499};
    VectorF vecOut_expect{-0.3636, -0.5392, -2.0700};

    QTest::newRow("Test 1") << vec1 << vec2 << vecOut_expect;
    return;
}

//----------
void VectorFHelperQTests::verify_cross_product()
{
    QFETCH(VectorF, vec1);
    QFETCH(VectorF, vec2);
    QFETCH(VectorF, vecOut_expect);

    VectorF vecOut = VectorFHelper::cross_product(vec1, vec2);
    QVERIFY(VectorFHelper::compare(vecOut, vecOut_expect, 1e-3));
    return;
}

//----------
void VectorFHelperQTests::verify_vec2D_cross_z_data()
{
    QTest::addColumn<VectorF>("vec");
    QTest::addColumn<VectorF>("vecOut_expect");

    VectorF vec{3.5784, 2.7694};
    VectorF vecOut_expect{2.7694, -3.5784};

    QTest::newRow("Test 1") << vec << vecOut_expect;
    return;
}

//----------
void VectorFHelperQTests::verify_vec2D_cross_z()
{
    QFETCH(VectorF, vec);
    QFETCH(VectorF, vecOut_expect);

    VectorF vecOut = VectorFHelper::vec2D_cross_z(vec);
    QVERIFY(VectorFHelper::compare(vecOut, vecOut_expect, 1e-3));
    return;
}



