#include "PolygonQTests.h"
#include <RrtPlannerLib/framework/algorithm/gjk/Polygon.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>


using namespace rrtplanner::framework;
using namespace rrtplanner::framework::algorithm::gjk;

//----------
PolygonQTests::PolygonQTests()
{

}

//----------
PolygonQTests::~PolygonQTests()
{
    cleanUp();
}

//----------
void PolygonQTests::setup()
{

}

//----------
void PolygonQTests::cleanUp()
{

}

//----------
void PolygonQTests::verify_support_data()
{
    QTest::addColumn<Polygon>("polygon");
    QTest::addColumn<VectorF>("dir");
    QTest::addColumn<VectorF>("support_expect");

    Polygon polygon({VectorF{0,0}, VectorF{-50,20}, VectorF{60, 30}, VectorF{50, 5}});

    //test 1
    VectorF dir{1, 0};
    VectorF support{60, 30};
    QTest::newRow("Test 1") << polygon << dir << support;

    //test 2
    dir = VectorF{0, -1};
    support = VectorF{0, 0};
    QTest::newRow("Test 2") << polygon << dir << support;
    return;
}

//----------
void PolygonQTests::verify_support()
{
    QFETCH(Polygon, polygon);
    QFETCH(VectorF, dir);
    QFETCH(VectorF, support_expect);

    VectorF support = polygon.support(dir);
    VectorF diff = VectorFHelper::subtract_vector(support, support_expect);
    QVERIFY(UtilHelper::compare(VectorFHelper::norm2(diff), 0.0));
    return;
}

