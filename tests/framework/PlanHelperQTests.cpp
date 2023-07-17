#include "PlanHelperQTests.h"
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>

using namespace rrtplanner::framework;

class MockPlan: public Plan
{
public:
    MockPlan()
        :Plan(){}
    virtual ~MockPlan(){}
    //in tests here, we may want to set a plan with zero length segment,
    //and skips the verify plan process, which disallows repeated successive waypoints.
    bool setPlan_skipCheck(const QVector<Waypt>& wayptList,
                           int id,
                           QString* resultsDesc = nullptr)
    {
        bool res_ok = setPlan(wayptList, QVector<int>(), resultsDesc, false);
        if(res_ok){
            setId(id);
        }
        return(res_ok);
    }
};

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
        //qInfo() << "Results >> " << wayptList.at(i) << "vs Expected >> " << wayptList_expect.at(i);
        QVERIFY(UtilHelper::compare(wayptList.at(i).northing(), wayptList_expect.at(i).northing()));
        QVERIFY(UtilHelper::compare(wayptList.at(i).easting(), wayptList_expect.at(i).easting()));
    }
    return;
}

//----------
void PlanHelperQTests::verify_pushPlan()
{
    QSharedPointer<Plan> p_plan1(new Plan), p_plan2(new Plan), p_plan3(new Plan);
    p_plan1->setId(1); p_plan2->setId(2); p_plan3->setId(3);
    QList<QSharedPointer<Plan>> planList{p_plan2};
    PlanHelper::pushPlan(p_plan1, -1.0, planList);
    PlanHelper::pushPlan(p_plan3, 1.0, planList);

    QCOMPARE(planList.at(0)->id(), 1);
    QCOMPARE(planList.at(1)->id(), 2);
    QCOMPARE(planList.at(2)->id(), 3);
}

//----------
void PlanHelperQTests::verify_buildSingleSideEllMaps_data()
{
    QTest::addColumn<MockPlan>("plan");
    QTest::addColumn<double>("side");
    QTest::addColumn<double>("crossTrackHorizon");
    QTest::addColumn<QVector<MockPlan>>("planList_expected");

    MockPlan plan;
    plan.setPlan_skipCheck(QVector<Waypt>{Waypt{0.0, 0.0},
                                Waypt{1000.0, 1000.0},
                                Waypt{2000.0, 1000.0},
                                Waypt{3000.0, 0.0},
                                Waypt{4000.0, 0.0}},
                 1);

    plan.setProperty(Plan::Property::IS_NOMINAL);

    //port side
    QVector<MockPlan> planList;
    MockPlan planTmp;
    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{1232.2330470, -1767.7669530},
                                   Waypt{1232.2330470, -1767.7669530},
                                   Waypt{1232.2330470, -1767.7669530},
                                   Waypt{1964.4660941, -2500.0000000},
                                   Waypt{4000.0000000, -2500.0000000}}, -1);
    planTmp.setCrossTrack(-2500.0000000);
    planTmp.setProperty(Plan::Property::IS_LIMIT);
    planList.push_back(planTmp);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{1500.0000000, -1500.0000000},
                                   Waypt{1500.0000000, -1500.0000000},
                                   Waypt{1500.0000000, -1500.0000000},
                                   Waypt{2121.3203436, -2121.3203436},
                                   Waypt{4000.0000000, -2121.3203436}}, -1);
    planTmp.setCrossTrack(-2121.3203436);
    planTmp.setProperty(Plan::Property::IS_LIMIT, false);
    planList.push_back(planTmp);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{853.5533906, -853.5533906},
                                   Waypt{1500.0000000, -207.1067812},
                                   Waypt{1500.0000000, -207.1067812},
                                   Waypt{2500.0000000, -1207.1067812},
                                   Waypt{4000.0000000, -1207.1067812}}, -1);
    planTmp.setCrossTrack(-1207.1067812);
    planTmp.setProperty(Plan::Property::IS_LIMIT, false);
    planList.push_back(planTmp);

    QTest::newRow("Test 1") << plan << -1.0 << 2500.0 << planList;

    //stbd side
    planList.clear();
    planList.push_back(plan);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{-1707.1067812, 1707.1067812},
                                   Waypt{-0.0000000, 3414.2135624},
                                   Waypt{3000.0000000, 3414.2135624},
                                   Waypt{4000.0000000, 2414.2135624},
                                   Waypt{4000.0000000, 2414.2135624}}, -1);
    planTmp.setCrossTrack(2414.2135624);
    planTmp.setProperty(Plan::Property::IS_LIMIT, false);
    planList.push_back(planTmp);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{-1767.7669530, 1767.7669530},
                                   Waypt{-35.5339059, 3500.0000000},
                                   Waypt{3035.5339059, 3500.0000000},
                                   Waypt{4060.6601718, 2474.8737342},
                                   Waypt{4060.6601718, 2474.8737342}}, -1);
    planTmp.setCrossTrack(2500.0000000);
    planTmp.setProperty(Plan::Property::IS_LIMIT);
    planList.push_back(planTmp);

    QTest::newRow("Test 2") << plan << 1.0 << 2500.0 << planList;
    return;
}

//----------
void PlanHelperQTests::verify_buildSingleSideEllMaps()
{
    QFETCH(MockPlan, plan);
    QFETCH(double, side);
    QFETCH(double, crossTrackHorizon);
    QFETCH(QVector<MockPlan>, planList_expected);

    QList<QSharedPointer<Plan>> planList;
    QString results_desc;
    bool result = PlanHelper::buildSingleSideEllMap(QSharedPointer<Plan>(new Plan(plan)), //nominal plan
                                    side, //-1.0 : port side, 1.0 : stbd side
                                    crossTrackHorizon, //[m] always a positive variable
                                    planList, //planList to prepend/append
                                    &results_desc);

    //qInfo() << "PlanHelper::buildSingleSideEllMaps results: " << results_desc;
    QVERIFY(result);
    QCOMPARE(planList.size(), planList_expected.size());
    for(int i = 0; i < planList.size(); ++i){
        //qInfo() << "RESULTS : " << *planList.at(i);
        //qInfo() << "EXPECTED: " << planList_expected.at(i);
        QCOMPARE(planList.at(i)->nSegment(), planList_expected.at(i).nSegment());
        QVERIFY(UtilHelper::compare(planList.at(i)->crossTrack(), planList_expected.at(i).crossTrack()));
        QVERIFY(UtilHelper::compare(planList.at(i)->length(), planList_expected.at(i).length()));
        for(int j = 0; j < planList.at(i)->nSegment(); ++j){
            const Segment& seg = planList.at(i)->segmentList().at(j);
            const Segment& seg_expected = planList_expected.at(i).segmentList().at(j);
            QVERIFY(UtilHelper::compare(seg.wayptPrev().easting(), seg_expected.wayptPrev().easting()));
            QVERIFY(UtilHelper::compare(seg.wayptPrev().northing(), seg_expected.wayptPrev().northing()));
            QVERIFY(UtilHelper::compare(seg.wayptNext().easting(), seg_expected.wayptNext().easting()));
            QVERIFY(UtilHelper::compare(seg.wayptNext().northing(), seg_expected.wayptNext().northing()));

            if(seg.length()> TOL_SMALL){
                QVERIFY(UtilHelper::compare(seg.tVec().at(IDX_NORTHING), seg_expected.tVec().at(IDX_NORTHING)));
                QVERIFY(UtilHelper::compare(seg.tVec().at(IDX_EASTING), seg_expected.tVec().at(IDX_EASTING)));
                QVERIFY(UtilHelper::compare(seg.nVec().at(IDX_NORTHING), seg_expected.nVec().at(IDX_NORTHING)));
                QVERIFY(UtilHelper::compare(seg.nVec().at(IDX_EASTING), seg_expected.nVec().at(IDX_EASTING)));
            }

            QVERIFY(UtilHelper::compare(seg.length(), seg_expected.length()));
            QVERIFY(UtilHelper::compare(seg.lengthCumulative(), seg_expected.lengthCumulative()));
        }
    }
}
