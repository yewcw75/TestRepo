#include "EllMapQTests.h"
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <QVector>


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
EllMapQTests::EllMapQTests()
{

}

//----------
EllMapQTests::~EllMapQTests()
{
    cleanUp();
}

//----------
void EllMapQTests::setup()
{

}

//----------
void EllMapQTests::cleanUp()
{

}

//----------
void EllMapQTests::verify_buildEllMap_data()
{
    QTest::addColumn<MockPlan>("planNominal");
    QTest::addColumn<double>("crossTrackHorizon");
    QTest::addColumn<QVector<MockPlan>>("planList_expect");

    //set nominal plan
    MockPlan planNominal;
    planNominal.setPlan_skipCheck(QVector<Waypt>{Waypt{0.0, 0.0, 0.0, 0},
                                Waypt{1000.0, 1000.0, 0.0, 1},
                                Waypt{2000.0, 1000.0, 0.0, 2},
                                Waypt{3000.0, 0.0, 0.0, 3},
                                Waypt{4000.0, 0.0, 0.0, 4}},
                 0);
    planNominal.setProperty(Plan::Property::IS_NOMINAL);

    //set expected ellmap plan list
    QVector<MockPlan> planList_expect;

    //port side
    MockPlan planTmp;
    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{1232.2330470, -1767.7669530, 0.0, 0},
                                   Waypt{1232.2330470, -1767.7669530, 0.0, 1},
                                   Waypt{1232.2330470, -1767.7669530, 0.0, 2},
                                   Waypt{1964.4660941, -2500.0000000, 0.0, 3},
                                   Waypt{4000.0000000, -2500.0000000, 0.0, 4}}, 0);
    planTmp.setCrossTrack(-2500.0000000);
    planTmp.setProperty(Plan::Property::IS_LIMIT);
    planList_expect.push_back(planTmp);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{1500.0000000, -1500.0000000, 0.0, 0},
                                   Waypt{1500.0000000, -1500.0000000, 0.0, 1},
                                   Waypt{1500.0000000, -1500.0000000, 0.0, 2},
                                   Waypt{2121.3203436, -2121.3203436, 0.0, 3},
                                   Waypt{4000.0000000, -2121.3203436, 0.0, 4}}, 1);
    planTmp.setCrossTrack(-2121.3203436);
    planTmp.setProperty(Plan::Property::IS_LIMIT, false);
    planList_expect.push_back(planTmp);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{853.5533906, -853.5533906, 0.0, 0},
                                   Waypt{1500.0000000, -207.1067812, 0.0, 1},
                                   Waypt{1500.0000000, -207.1067812, 0.0, 2},
                                   Waypt{2500.0000000, -1207.1067812, 0.0, 3},
                                   Waypt{4000.0000000, -1207.1067812, 0.0, 4}}, 2);
    planTmp.setCrossTrack(-1207.1067812);
    planTmp.setProperty(Plan::Property::IS_LIMIT, false);
    planList_expect.push_back(planTmp);

    //stbd side
    planList_expect.push_back(planNominal);
    planList_expect.last().setId(3);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{-1707.1067812, 1707.1067812, 0.0, 0},
                                   Waypt{-0.0000000, 3414.2135624, 0.0, 1},
                                   Waypt{3000.0000000, 3414.2135624, 0.0, 2},
                                   Waypt{4000.0000000, 2414.2135624, 0.0, 3},
                                   Waypt{4000.0000000, 2414.2135624, 0.0, 4}}, 4);
    planTmp.setCrossTrack(2414.2135624);
    planTmp.setProperty(Plan::Property::IS_LIMIT, false);
    planList_expect.push_back(planTmp);

    planTmp.setPlan_skipCheck(QVector<Waypt>{Waypt{-1767.7669530, 1767.7669530, 0.0, 0},
                                   Waypt{-35.5339059, 3500.0000000, 0.0, 1},
                                   Waypt{3035.5339059, 3500.0000000, 0.0, 2},
                                   Waypt{4060.6601718, 2474.8737342, 0.0, 3},
                                   Waypt{4060.6601718, 2474.8737342, 0.0, 4}}, 5);
    planTmp.setCrossTrack(2500.0000000);
    planTmp.setProperty(Plan::Property::IS_LIMIT);
    planList_expect.push_back(planTmp);

    QTest::newRow("Test 1") << planNominal << 2500.0 << planList_expect;
    
}

//----------
void EllMapQTests::verify_buildEllMap()
{
    QFETCH(MockPlan, planNominal);
    QFETCH(double, crossTrackHorizon);
    QFETCH(QVector<MockPlan>, planList_expect);

    EllMap ellMap;
    ellMap.setNominalPlan(planNominal);
    QString res_desc;
    bool res_ok = ellMap.buildEllMap(crossTrackHorizon, &res_desc);

    //qInfo() << "Results description: " << res_desc;
    QVERIFY(res_ok);

    //Display ellmaps
    //qInfo() << ellMap;

    //compare plans with expected results
    QCOMPARE(ellMap.nPlan(), planList_expect.size());
    for(int i = 0; i < ellMap.nPlan(); ++i){
        //qInfo() << "RESULTS : " << *planList.at(i);
        //qInfo() << "EXPECTED: " << planList_expected.at(i);
        QCOMPARE(ellMap.at(i)->id(), planList_expect.at(i).id());
        QCOMPARE(ellMap.at(i)->nSegment(), planList_expect.at(i).nSegment());
        QVERIFY(UtilHelper::compare(ellMap.at(i)->crossTrack(), planList_expect.at(i).crossTrack()));
        QVERIFY(UtilHelper::compare(ellMap.at(i)->length(), planList_expect.at(i).length()));
        for(int j = 0; j < ellMap.at(i)->nSegment(); ++j){
            const Segment& seg = ellMap.at(i)->segmentList().at(j);
            const Segment& seg_expected = planList_expect.at(i).segmentList().at(j);
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

//----------
void EllMapQTests::verify_locateSector_data()
{
    QTest::addColumn<Plan>("planNominal");
    QTest::addColumn<double>("crossTrackHorizon");
    QTest::addColumn<VectorF>("posNE");
    QTest::addColumn<int>("planIdx_expect");
    QTest::addColumn<int>("segIdx_expect");

    //set nominal plan
    Plan planNominal;
    planNominal.setPlan(QVector<Waypt>{Waypt{0.0, 0.0, 0.0, 0},
                                Waypt{1000.0, 1000.0, 0.0, 1},
                                Waypt{2000.0, 1000.0, 0.0, 2},
                                Waypt{3000.0, 0.0, 0.0, 3},
                                Waypt{4000.0, 0.0, 0.0, 4}},
                 0);
    planNominal.setProperty(Plan::Property::IS_NOMINAL);
    QTest::newRow("Test 1") << planNominal << 2500.0 << VectorF{2200, 0} << 2 << 2;
}

//----------
void EllMapQTests::verify_locateSector()
{
    QFETCH(Plan, planNominal);
    QFETCH(double, crossTrackHorizon);
    QFETCH(VectorF, posNE);
    QFETCH(int, planIdx_expect);
    QFETCH(int, segIdx_expect);

    EllMap ellMap;
    ellMap.setNominalPlan(planNominal);
    QString res_desc;
    bool res_ok = ellMap.buildEllMap(crossTrackHorizon, &res_desc);

    //qInfo() << "Results description: " << res_desc;
    QVERIFY(res_ok);

    int planIdx, segIdx;
    bool found = ellMap.locateSector(posNE, 0, 0, planIdx, segIdx);

    QVERIFY(found);
    QCOMPARE(planIdx, planIdx_expect);
    QCOMPARE(segIdx, segIdx_expect);
}

//----------
void EllMapQTests::verify_getRootData_data()
{
    QTest::addColumn<Plan>("planNominal");
    QTest::addColumn<double>("crossTrackHorizon");
    QTest::addColumn<VectorF>("posNE");
    QTest::addColumn<int>("planIdx_expect");
    QTest::addColumn<int>("segIdx_expect");
    QTest::addColumn<double>("dx_expect");
    QTest::addColumn<double>("ell_expect");
    QTest::addColumn<double>("f_ell_expect");
    QTest::addColumn<double>("L_expect");
    QTest::addColumn<bool>("isInPoly_expect");
    QTest::addColumn<QVector<double>>("ell_list_expect");

    //set nominal plan
    Plan planNominal;
    planNominal.setPlan(QVector<Waypt>{Waypt{0.0, 0.0, 0.0, 0},
                                Waypt{1000.0, 1000.0, 0.0, 1},
                                Waypt{2000.0, 1000.0, 0.0, 2},
                                Waypt{3000.0, 0.0, 0.0, 3},
                                Waypt{4000.0, 0.0, 0.0, 4}},
                 0);
    double crossTrackHorizon = 2500.0;
    VectorF posNE{2200, 00};
    int planIdx_expect = 2;
    int segIdx_expect = 2;
    double dx_expect = -565.6854;
    double ell_expect = 2325.4834;
    double f_ell_expect = 0.4343;
    double L_expect = 1414.2136;
    bool isInPoly_expect = true;
    QVector<double> ell_list_expect{449.7475, 381.6234, 1528.4271, 3028.4271, 6028.4271, 6150.4617};
    planNominal.setProperty(Plan::Property::IS_NOMINAL);
    QTest::newRow("Test 1") << planNominal << crossTrackHorizon << posNE << planIdx_expect << segIdx_expect << \
                               dx_expect << ell_expect << f_ell_expect << L_expect << isInPoly_expect << ell_list_expect;
}

//----------
void EllMapQTests::verify_getRootData()
{
    QFETCH(Plan, planNominal);
    QFETCH(double, crossTrackHorizon);
    QFETCH(VectorF, posNE);
    QFETCH(int, planIdx_expect);
    QFETCH(int, segIdx_expect);
    QFETCH(double, dx_expect);
    QFETCH(double, ell_expect);
    QFETCH(double, f_ell_expect);
    QFETCH(double, L_expect);
    QFETCH(bool, isInPoly_expect);
    QFETCH(QVector<double>, ell_list_expect);

    EllMap ellMap;
    ellMap.setNominalPlan(planNominal);
    QString res_desc;
    bool res_ok = ellMap.buildEllMap(crossTrackHorizon, &res_desc);

    //qInfo() << "Results description: " << res_desc;
    QVERIFY(res_ok);

    RootData rootData;
    bool found = ellMap.getRootData(posNE, rootData);

    //qInfo() << "rootData:" << rootData;

    QVERIFY(found);
    QCOMPARE(rootData.planIdx(), planIdx_expect);
    QCOMPARE(rootData.segIdx(), segIdx_expect);

    QVERIFY(UtilHelper::compare(rootData.dx(), dx_expect, 1e-3));
    QVERIFY(UtilHelper::compare(rootData.ell(), ell_expect, 1e-3));
    QVERIFY(UtilHelper::compare(rootData.f_ell(), f_ell_expect, 1e-3));
    QVERIFY(UtilHelper::compare(rootData.L(), L_expect, 1e-3));
    QCOMPARE(rootData.isInPoly(), isInPoly_expect);

    const QVector<double>& ell_list = rootData.ell_list_const_ref();
    QCOMPARE(ell_list.size(), ell_list_expect.size());
    for(int i = 0; i < ell_list.size(); ++i){
        QVERIFY(UtilHelper::compare(ell_list.at(i), ell_list_expect.at(i), 1e-3));
    }
}

