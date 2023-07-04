#include "PlanQTests.h"
#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>


using namespace rrtplanner::framework;

//----------
PlanQTests::PlanQTests()
{

}

//----------
PlanQTests::~PlanQTests()
{
    cleanUp();
}

//----------
void PlanQTests::setup()
{

}

//----------
void PlanQTests::cleanUp()
{

}

//----------
void PlanQTests:: verifySame(const Waypt& waypt1, const Waypt& waypt2)
{
    QCOMPARE(waypt1.id(), waypt2.id());
    QCOMPARE(waypt1.northing(), waypt2.northing());
    QCOMPARE(waypt1.easting(), waypt2.easting());
    QCOMPARE(waypt1.lon0_deg(), waypt2.lon0_deg());
    return;
}

//----------
void PlanQTests::verify_set_plan_data()
{
    QTest::addColumn<int>("id");
    QTest::addColumn<QVector<Waypt>>("wayptList");
    QTest::addColumn<QVector<int>>("segIdList");
    QTest::addColumn<double>("planLength");
    QTest::addColumn<double>("crossTrack");
    QTest::addColumn<int>("propertyFlagsVal");
    QTest::addColumn<bool>("setOk");
    QTest::addColumn<QVector<VectorF>>("bVecList");

    QVector<Waypt> wayptList;
    wayptList.append(Waypt(0, 0, 103, 0));
    wayptList.append(Waypt(1000, 1000, 103, 1));
    wayptList.append(Waypt(2000, 1000, 103, 2));
    wayptList.append(Waypt(3000, 0, 103, 3));
    wayptList.append(Waypt(4000, 0, 103, 3));

    QVector<VectorF> bVecList;
    bVecList.append(VectorF{-0.707107, 0.707107});
    bVecList.append(VectorF{-0.382683, 0.923880});
    bVecList.append(VectorF{0.382683, 0.923880});
    bVecList.append(VectorF{0.382683, 0.923880});
    bVecList.append(VectorF{0.0, 1.0});

    Plan::PropertyFlags propertyFlags = Plan::Property::IS_LIMIT;
    QTest::newRow("Test 1A (empty SegIdList)") << (int) 1 << (QVector<Waypt>)wayptList << QVector<int>() << \
                               (double)4828.42712475 << (double)10.0 << (int)propertyFlags << (bool)true << \
                               bVecList;

    QTest::newRow("Test 1B (specified SegIdList)") << (int) 1 << (QVector<Waypt>)wayptList << QVector<int>{11, 22, 33, 44} << \
                               (double)4828.42712475 << (double)10.0 << (int)propertyFlags << (bool)true << \
                               bVecList;

    QTest::newRow("Test 1C (specified SegIdList with wrong length)") << (int) 1 << (QVector<Waypt>)wayptList << QVector<int>{11, 22, 33, 44, 55} << \
                               (double)4828.42712475 << (double)10.0 << (int)propertyFlags << (bool)false << \
                               bVecList;

    QVector<Waypt> wayptList2;
    propertyFlags = Plan::Property::IS_NOMINAL;
    wayptList2.append(Waypt(0, 0, 103, 0));
    QTest::newRow("Test 2") << (int) 1 << (QVector<Waypt>)wayptList2 << QVector<int>() <<\
                               (double)0.0 << (double)-10.0 << (int)propertyFlags << (bool)false << \
                               bVecList /*does not matter*/;

    QVector<Waypt> wayptList3;
    wayptList3.append(Waypt(0, 0, 103, 0));
    wayptList3.append(Waypt(1000, 1000, 103, 1));
    wayptList3.append(Waypt(950, 1500, 103, 1)); //go back a little
    wayptList3.append(Waypt(2000, 1500, 103, 2));
    wayptList3.append(Waypt(3000, 0, 103, 3));
    wayptList3.append(Waypt(4000, 0, 103, 3));
    propertyFlags = Plan::Property::NONE;
    QTest::newRow("Test 3") << (int) 1 << (QVector<Waypt>)wayptList3 << QVector<int>() <<\
                               (double)0.0 /*does not matter, expected not used*/ << (double)10.0 << (int)propertyFlags << (bool)false << \
                               bVecList /*does not matter*/;
    return;
}

//----------
void PlanQTests::verify_set_plan()
{
    setup();

    QFETCH(int, id);
    QFETCH(QVector<Waypt>, wayptList);
    QFETCH(QVector<int>, segIdList);
    QFETCH(double, planLength);
    QFETCH(double, crossTrack);
    QFETCH(int, propertyFlagsVal);
    QFETCH(bool, setOk);
    QFETCH(QVector<VectorF>, bVecList);

    //set plan
    Plan plan;
    QString resString;
    bool res = plan.setPlan(wayptList, segIdList, id, &resString);
    //qInfo() << "[PlanQTests::verify_set_plan()]" << resString;
    QCOMPARE(res, setOk);

    //set crosstrack
    plan.setCrossTrack(crossTrack);
    QCOMPARE(plan.crossTrack(), crossTrack);

    //set property flags
    Plan::PropertyFlags propertyFlags(propertyFlagsVal);
    plan.setPropertyFlags(propertyFlags);
    QCOMPARE(plan.propertyFlags(), propertyFlags);

    if(res){
        //verify waypts are set correctly
        QVector<Waypt> wayptList_res = plan.wayptList();
        for(int i = 0; i < wayptList.size() && i < wayptList_res.size(); ++i){
            verifySame(wayptList.at(i), wayptList_res.at(i));
        }

        //verify bisectors
        QVector<Segment> segmentList_res = plan.segmentList();
        QCOMPARE(segmentList_res.size() + 1, bVecList.size());
        for(int i = 0; i < segmentList_res.size(); ++i){
            const Segment& currSegment = segmentList_res.at(i);
            QVERIFY(UtilHelper::compare(currSegment.bVecPrev()[IDX_NORTHING], bVecList.at(i)[IDX_NORTHING]));
            QVERIFY(UtilHelper::compare(currSegment.bVecPrev()[IDX_EASTING], bVecList.at(i)[IDX_EASTING]));
            QVERIFY(UtilHelper::compare(currSegment.bVecNext()[IDX_NORTHING], bVecList.at(i+1)[IDX_NORTHING]));
            QVERIFY(UtilHelper::compare(currSegment.bVecNext()[IDX_EASTING], bVecList.at(i+1)[IDX_EASTING]));
        }

        //verify other params
        QCOMPARE(plan.id(), id);
        QVERIFY(UtilHelper::compare(plan.length(), planLength));
        QCOMPARE(plan.nWaypt(), wayptList.size());
        QCOMPARE(plan.nSegment(), wayptList.size() - 1);

        //verify segment id are set correctly
        for(int i = 0; i < plan.segmentList().size(); ++i){
            if(segIdList.size() == 0){
                QCOMPARE(plan.segmentList().at(i).id(), i);
            }
            else{
                QCOMPARE(plan.segmentList().at(i).id(), segIdList.at(i));
            }
        }
    }
}
