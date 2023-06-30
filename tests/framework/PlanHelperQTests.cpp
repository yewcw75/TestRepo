#include "PlanHelperQTests.h"
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>

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
    QTest::addColumn<float>("side");
    QTest::addColumn<float>("crossTrackHorizon");
    QTest::addColumn<float>("dxNearest_expect");
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
    QTest::newRow("Test 1") << plan << -1.0f << 2500.0f << -1207.106781f << eventSegIdxList;

    eventSegIdxList = QVector<int>{3}; //note: matlab idx from 1. in c, our idx is from 0.
    QTest::newRow("Test 2") << plan << 1.0f << 2500.0f << 2414.213562f << eventSegIdxList;
}

//----------
void PlanHelperQTests::verify_findNearestEdgeEventdata()
{
    QFETCH(Plan, plan);
    QFETCH(float, side);
    QFETCH(float, crossTrackHorizon);
    QFETCH(float, dxNearest_expect);
    QFETCH(QVector<int>, eventSegIdxList_expect);

    float dxNearest;
    QVector<int> eventSegIdxList;
    bool found =  PlanHelper::findNearestEdgeEvent(plan, crossTrackHorizon, side, 1e-6, dxNearest, eventSegIdxList);

    QCOMPARE(found, eventSegIdxList.size() > 0);
    if(found){
       QVERIFY(qFuzzyCompare(dxNearest, dxNearest_expect));
       QCOMPARE(eventSegIdxList.size(), eventSegIdxList_expect.size());
       for(auto& value: eventSegIdxList){
           QVERIFY(eventSegIdxList_expect.contains(value));
       }
    }
    return;
}
