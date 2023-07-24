#include "GjkQTests.h"
#include <RrtPlannerLib/framework/algorithm/gjk/GjkFactory.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Polygon.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>


//----------
GjkQTests::GjkQTests()
    :mp_gjk(GjkFactory::getGjk(GjkFactory::GjkType::Basic)),
      mp_gjkMinDist(GjkFactory::getGjk(GjkFactory::GjkType::MinDist))
{

}

//----------
GjkQTests::~GjkQTests()
{
    cleanUp();
}

//----------
void GjkQTests::setup()
{

}

//----------
void GjkQTests::cleanUp()
{

}

//----------
void GjkQTests::verify_chkIntersect_data()
{
    QTest::addColumn<Polygon>("polygon1");
    QTest::addColumn<Polygon>("polygon2");
    QTest::addColumn<bool>("intersect_expect");
    QTest::addColumn<double>("distance_expect");
    QTest::addColumn<bool>("distance_valid_expect");

    //test 1
    Polygon polygon1{VectorF{-30.0184,9.9299}, VectorF{-30.9392,-6.4252}, VectorF{-19.7053,-19.2757}, VectorF{-7.1823,1.0514}, VectorF{-5.8932,16.9393}, VectorF{-30.0184,9.9299}};
    Polygon polygon2{VectorF{23.0203,9.9299}, VectorF{9.2081,-15.5374}, VectorF{54.8803,-33.5280}, VectorF{23.0203,9.9299}};
    bool intersect_expect = false;
    double distance_expect = 22.3165;
    bool distance_valid_expect = true;
    QTest::newRow("Test 1 - no intersect") << polygon1 << polygon2 << intersect_expect << distance_expect << distance_valid_expect;

    //test 2
    polygon1 = Polygon{VectorF{-30.2026,19.7430}, VectorF{-30.0184,0.3505}, VectorF{-10.1289,-0.5841}, VectorF{-9.5764,19.7430}, VectorF{-30.2026,19.7430}};
    polygon2 = Polygon{VectorF{-9.3923,19.2757}, VectorF{-10.1289,-0.8178}, VectorF{10.1289,-0.3505}, VectorF{9.9448,19.7430}, VectorF{-9.3923,19.2757}};
    intersect_expect = false;
    distance_expect = 0.0086;
    distance_valid_expect = true;
    QTest::newRow("Test 2 - no intersect") << polygon1 << polygon2 << intersect_expect<< distance_expect << distance_valid_expect;

    //test 3
    polygon1 = Polygon{VectorF{-20.6262,13.9019}, VectorF{-18.2320,-7.1262}, VectorF{13.0755,-5.9579}, VectorF{-20.6262,13.9019}};
    polygon2 = Polygon{VectorF{7.9190,17.1729}, VectorF{1.6575,0.5841}, VectorF{31.4917,0.5841}, VectorF{36.6483,23.0140}, VectorF{7.9190,17.1729}};
    intersect_expect = true;
    distance_expect = 0.0;
    distance_valid_expect = false;
    QTest::newRow("Test 3 - intersect") << polygon1 << polygon2 << intersect_expect << distance_expect << distance_valid_expect;

    //test 4
    polygon1 = Polygon{VectorF{-32.4125,24.4159}, VectorF{-27.9926,-0.8178}, VectorF{-11.0497,-16.4720}, VectorF{6.6298,15.5374}, VectorF{-32.4125,24.4159}};
    polygon2 = Polygon{VectorF{-6.4457,8.9953}, VectorF{-19.7053,12.9673}, VectorF{-17.6796,-1.7523}, VectorF{-6.4457,8.9953}};
    intersect_expect = true;
    distance_expect = 0.0;
    distance_valid_expect = false;
    QTest::newRow("Test 4 - intersect") << polygon1 << polygon2 << intersect_expect<< distance_expect << distance_valid_expect;
    return;
}

//----------
void GjkQTests::verify_chkIntersect()
{
    QFETCH(Polygon, polygon1);
    QFETCH(Polygon, polygon2);
    QFETCH(bool, intersect_expect);
    QFETCH(double, distance_expect);
    QFETCH(bool, distance_valid_expect);

    double distance;
    bool isValidDistance;
    bool intersect = mp_gjk->chkIntersect(polygon1, polygon2, distance, isValidDistance);

    QCOMPARE(intersect, intersect_expect);
    QCOMPARE(isValidDistance, false); //basic version will not hv distance


    bool intersect2 = mp_gjkMinDist->chkIntersect(polygon1, polygon2, distance, isValidDistance);
    QCOMPARE(intersect2, intersect_expect);
    QCOMPARE(isValidDistance, distance_valid_expect);
    if(isValidDistance){
        QVERIFY(UtilHelper::compare(distance, distance_expect, 1e-3));
    }
    return;
}
