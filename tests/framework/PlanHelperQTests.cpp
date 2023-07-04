#include "PlanHelperQTests.h"
#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>

using namespace rrtplanner::framework;

//----------
PlanHelperQTests::PlanHelperQTests()
{

}

//----------
PlanHelperQTests::~PlanHelperQTests()
{
    cleanUp();
}

//----------
void PlanHelperQTests::setup()
{

}

//----------
void PlanHelperQTests::cleanUp()
{

}

//----------
void PlanHelperQTests::verify_findNearestEdgeEventdata_data()
{
    QTest::addColumn<Plan>("plan");
    QTest::addColumn<double>("side");
    QTest::addColumn<double>("crossTrackHorizon");
    QTest::addColumn<double>("dxNearest_expect");
    QTest::addColumn<QVector<int>>("eventSegIdxList_expect");

    Plan plan;
    plan.setPlan(QVector<Waypt>{Waypt{0.0, 0.0},
                                Waypt{1000.0, 1000.0},
                                Waypt{2000.0, 1000.0},
                                Waypt{3000.0, 0.0},
                                Waypt{4000.0, 0.0}},
                 1);
    QVector<int> eventSegIdxList{1}; //note: matlab idx from 1. in c, our idx is from 0.
    plan.setProperty(Plan::Property::IS_NOMINAL);
    QTest::newRow("Test 1") << plan << -1.0 << 2500.0 << -1207.106781 << eventSegIdxList;

    eventSegIdxList = QVector<int>{3}; //note: matlab idx from 1. in c, our idx is from 0.
    QTest::newRow("Test 2") << plan << 1.0 << 2500.0 << 2414.213562 << eventSegIdxList;
    return;
}

//----------
void PlanHelperQTests::verify_findNearestEdgeEventdata()
{
    QFETCH(Plan, plan);
    QFETCH(double, side);
    QFETCH(double, crossTrackHorizon);
    QFETCH(double, dxNearest_expect);
    QFETCH(QVector<int>, eventSegIdxList_expect);

    double dxNearest;
    QVector<int> eventSegIdxList;
    bool found =  PlanHelper::findNearestEdgeEvent(plan, crossTrackHorizon, side, 1e-6, dxNearest, eventSegIdxList);

    QCOMPARE(found, eventSegIdxList.size() > 0);
    if(found){
       QVERIFY(UtilHelper::compare(dxNearest, dxNearest_expect));
       QCOMPARE(eventSegIdxList.size(), eventSegIdxList_expect.size());
       for(auto& value: eventSegIdxList){
           QVERIFY(eventSegIdxList_expect.contains(value));
       }
    }
    return;
}

//----------
void PlanHelperQTests::verify_getCrossTrackPlan_data()
{
    QTest::addColumn<Plan>("plan");
    QTest::addColumn<double>("crossTrackHorizon");
    QTest::addColumn<double>("dx");
    QTest::addColumn<QVector<int>>("eventSegIdxList");
    QTest::addColumn<QVector<Waypt>>("wayptList_expect");

    Plan plan;
    plan.setPlan(QVector<Waypt>{Waypt{0.0, 0.0},
                                Waypt{1000.0, 1000.0},
                                Waypt{2000.0, 1000.0},
                                Waypt{3000.0, 0.0},
                                Waypt{4000.0, 0.0}},
                 1);
    QVector<int> eventSegIdxList{1}; //note: matlab idx from 1. in c, our idx is from 0.
    QVector<Waypt> wayptList_expect{Waypt{853.553391, -853.553391},
                                    Waypt{1500.000000, -207.106781},
                                    Waypt{2500.000000, -1207.106781},
                                    Waypt{4000.000000, -1207.106781}};
    QTest::newRow("Test 1") << plan << 2500.0 << -1207.106781 << eventSegIdxList << wayptList_expect;

    eventSegIdxList = QVector<int>{3}; //note: matlab idx from 1. in c, our idx is from 0.
    wayptList_expect = QVector<Waypt>{Waypt{-1707.106781, 1707.106781},
                                Waypt{0.000000, 3414.213562},
                                Waypt{3000.000000, 3414.213562},
                                Waypt{4000.000000, 2414.213562}};
    QTest::newRow("Test 2") << plan <<  2500.0 << 2414.213562 << eventSegIdxList << wayptList_expect;
    return;
}


//----------
void PlanHelperQTests::verify_getCrossTrackPlan()
{
    QFETCH(Plan, plan);
    QFETCH(double, crossTrackHorizon);
    QFETCH(double, dx);
    QFETCH(QVector<int>, eventSegIdxList);
    QFETCH(QVector<Waypt>, wayptList_expect);

    bool results_out;
    Plan plan_offset = PlanHelper::getCrossTrackPlan(plan, crossTrackHorizon, dx, eventSegIdxList, 1e-6, &results_out);
    const QVector<Waypt>& wayptList = plan_offset.wayptList();

    QCOMPARE(wayptList.size(), wayptList_expect.size());
    for(int i = 0; i < wayptList.size(); ++i){
        qInfo() << "Results >> " << wayptList.at(i) << "vs Expected >> " << wayptList_expect.at(i);
        QVERIFY(UtilHelper::compare(wayptList.at(i).northing(), wayptList_expect.at(i).northing()));
        QVERIFY(UtilHelper::compare(wayptList.at(i).easting(), wayptList_expect.at(i).easting()));
    }
    return;
}
