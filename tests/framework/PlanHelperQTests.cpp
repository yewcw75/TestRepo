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
    return;
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

//----------
void PlanHelperQTests::verify_getCrossTrackPlan_data()
{
    QTest::addColumn<Plan>("plan");
    QTest::addColumn<float>("crossTrackHorizon");
    QTest::addColumn<float>("dx");
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
    QTest::newRow("Test 1") << plan << 2500.0f << -1207.106781f << eventSegIdxList << wayptList_expect;

    eventSegIdxList = QVector<int>{3}; //note: matlab idx from 1. in c, our idx is from 0.
    wayptList_expect = QVector<Waypt>{Waypt{-1707.106781, 1707.106781},
                                Waypt{0.000000, 3414.213562},
                                Waypt{3000.000000, 3414.213562},
                                Waypt{4000.000000, 2414.213562}};
    QTest::newRow("Test 2") << plan <<  2500.0f << 2414.213562f << eventSegIdxList << wayptList_expect;
    return;
}


//----------
void PlanHelperQTests::verify_getCrossTrackPlan()
{
    QFETCH(Plan, plan);
    QFETCH(float, crossTrackHorizon);
    QFETCH(float, dx);
    QFETCH(QVector<int>, eventSegIdxList);
    QFETCH(QVector<Waypt>, wayptList_expect);

    bool results_out;
    Plan plan_offset = PlanHelper::getCrossTrackPlan(plan, crossTrackHorizon, dx, eventSegIdxList, 1e-6, &results_out);
    const QVector<Waypt>& wayptList = plan_offset.wayptList();

    QCOMPARE(wayptList.size(), wayptList_expect.size());
    for(int i = 0; i < wayptList.size(); ++i){
        qInfo() << "Results >> " << wayptList.at(i) << "vs Expected >> " << wayptList_expect.at(i);
        QVERIFY(qFuzzyCompare(1.0+wayptList.at(i).northing(), 1.0+wayptList_expect.at(i).northing()));
        QVERIFY(qFuzzyCompare(1.0+wayptList.at(i).easting(), 1.0+wayptList_expect.at(i).easting()));
    }
    return;
}
