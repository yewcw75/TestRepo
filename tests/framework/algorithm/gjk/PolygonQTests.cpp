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

//----------
void PolygonQTests::verify_clone_data()
{
    QTest::addColumn<Polygon>("polygon");

    Polygon polygon({VectorF{1, 1}, VectorF{-50,20}, VectorF{60, 30}, VectorF{50, 5}});

    //test 1
    QTest::newRow("Test 1") << polygon;

    return;
}

//----------
void PolygonQTests::verify_clone()
{
    QFETCH(Polygon, polygon);

    IShape* shape = &polygon;
    IShape* shape2 = shape->clone(); //clone at base class level
    Polygon* polygon2 = dynamic_cast<Polygon*>(shape2);
    polygon2->operator[](0) = VectorFHelper::add_vector(polygon.at(0), VectorF{1,1});

    //qInfo() << "Polygon:" << *shape;
    //qInfo() << "Polygon2:" << *shape2;

    QVERIFY(!VectorFHelper::compare(polygon2->at(0), polygon.at(0), 1e-6));
    for(int i = 1; i < polygon.size(); ++i){
        QVERIFY(VectorFHelper::compare(polygon2->at(i), polygon.at(i), 1e-6));
    }
    return;
}

//----------
void PolygonQTests::verify_centroid_data()
{
    QTest::addColumn<Polygon>("polygon");
    QTest::addColumn<VectorF>("centroid_expect");

    Polygon polygon({VectorF{1.0, 1.0}, VectorF{2.0, 4.0}, VectorF{5.0, 3.0}, VectorF{4.0, 1.0}});
    VectorF centroid_expect{3, 2.25};

    //test 1
    QTest::newRow("Test 1") << polygon << centroid_expect;

    return;
}

//----------
void PolygonQTests::verify_centroid()
{
    QFETCH(Polygon, polygon);
    QFETCH(VectorF, centroid_expect);

    VectorF centroid = polygon.centroid();

    qInfo() << "centroid: " << centroid;
    qInfo() << "centroid_expect: " << centroid_expect;
    QVERIFY(VectorFHelper::compare(centroid, centroid_expect, 1e-6));
    return;
}

