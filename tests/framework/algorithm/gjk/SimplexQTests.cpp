#include "SimplexQTests.h"
#include <RrtPlannerLib/framework/algorithm/gjk/Simplex.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>


using namespace rrtplanner::framework;
using namespace rrtplanner::framework::algorithm::gjk;

//----------
SimplexQTests::SimplexQTests()
{

}

//----------
SimplexQTests::~SimplexQTests()
{
    cleanUp();
}

//----------
void SimplexQTests::setup()
{

}

//----------
void SimplexQTests::cleanUp()
{

}

//----------
void SimplexQTests::verify_update_data()
{
    QTest::addColumn<QVector<VectorF>>("vertexList");
    QTest::addColumn<QVector<bool>>("resultsList_expect");

    //test 1
    QVector<VectorF> vertexList{VectorF{-10.4972,-1.8692}, VectorF{69.9816,39.4860}, VectorF{44.3831,-16.1215}};
    QVector<bool> resultsList_expect{false,false,true};
    QTest::newRow("Test 1") << vertexList << resultsList_expect;

    //test 2
    vertexList = QVector<VectorF>{VectorF{-0.5525,8.8785}, VectorF{5.1565,-22.4299}};
    resultsList_expect = QVector<bool>{false,false};
    QTest::newRow("Test 2") << vertexList << resultsList_expect;

    //test 3
    vertexList = QVector<VectorF>{VectorF{-32.4125,-2.3364}, VectorF{32.4125,7.9439}, VectorF{-5.8932,-33.1776}};
    resultsList_expect = QVector<bool>{false,false,true};
    QTest::newRow("Test 3") << vertexList << resultsList_expect;

    return;
}

//----------
void SimplexQTests::verify_update()
{
    QFETCH(QVector<VectorF>, vertexList);
    QFETCH(QVector<bool>, resultsList_expect);

    Simplex simplex;
    for(int i = 0; i < vertexList.size(); ++i){
        VectorF v;
        bool results = simplex.update(vertexList.at(i), v);
        QCOMPARE(results, resultsList_expect.at(i));
    }
    return;
}
