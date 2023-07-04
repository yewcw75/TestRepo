#include "VectorFQTests.h"
#include <RrtPlannerLib/framework/VectorF.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <QVector>
#include <iostream>

using namespace rrtplanner::framework;
namespace bnu = boost::numeric::ublas;

//----------
VectorFQTests::VectorFQTests()
{

}

//----------
VectorFQTests::~VectorFQTests()
{

}

//----------
void VectorFQTests::verify_constructors()
{
    VectorF v1;
    QCOMPARE(v1.size(), 0);

    VectorF v3{1.0, 2.0, 3.3, -5.5};
    //qInfo() << "v3: " << v3;
    QCOMPARE(v3.size(), 4);

    bnu::vector<double> data(3);
    data[0] = 0.1; data[1] = 0.2; data[2] = 0.3;
    VectorF v4(data);
    QCOMPARE(v4.size(), 3);
    for(int i = 0; i < (int)data.size(); ++i){
        QCOMPARE(v4[i], data[i]);
    }
}

//----------
void VectorFQTests::verify_copy()
{
    VectorF v1{1.0, 2.0, 3.3, -5.5};
    //qInfo() << "v1: " << v1;
    QCOMPARE(v1.size(), 4);

    VectorF v2(v1);
    //qInfo() << "v2: " << v2;
    for(int i = 0; i < v1.size(); ++ i){
        QCOMPARE(v2[i], v1[i]);
    }

    VectorF v3 = v1;
    //qInfo() << "v3: " << v3;
    for(int i = 0; i < v1.size(); ++ i){
        QCOMPARE(v3[i], v1[i]);
    }
}

//----------
void VectorFQTests::verify_resize()
{
    VectorF v1{1.0, 2.0, 3.3, -5.5};
    //qInfo() << "v1: " << v1;
    QCOMPARE(v1.size(), 4);
    v1.resize(6);
    //qInfo() << "v1: " << v1;
    QCOMPARE(v1.size(), 6);
}

//----------
void VectorFQTests::verify_access()
{
    const VectorF v1{1.0, 2.0, 3.3, -5.5};
    const double& f1 = v1[1];
    QCOMPARE(f1, 2.0);

    VectorF v2{1.0, 2.0, 3.3, -5.5};
    double& f2 = v2[1];
    QCOMPARE(f2, 2.0);

    double f3 = v2.at(3);
    QCOMPARE(f3, -5.5);
}

