#include "SMapHelperQTests.h"
#include <RrtPlannerLib/framework/SMapHelper.h>
#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <RrtPlannerLib/framework/EllMap.h>
#include <RrtPlannerLib/framework/RootData.h>
#include <RrtPlannerLib/framework/VectorF.h>
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
SMapHelperQTests::SMapHelperQTests()
{

}

//----------
SMapHelperQTests::~SMapHelperQTests()
{
    cleanUp();
}

//----------
void SMapHelperQTests::setup()
{

}

//----------
void SMapHelperQTests::cleanUp()
{

}

//----------
void SMapHelperQTests::verify_determineArcLengthHorizon_data()
{
    QTest::addColumn<MockPlan>("plan");
    QTest::addColumn<double>("ell0");
    QTest::addColumn<double>("lh0");
    QTest::addColumn<double>("lh_expect");
    QTest::addColumn<double>("ellMax_expect");

    //Test 1
    MockPlan plan;
    plan.setPlan_skipCheck(QVector<Waypt>{Waypt{1232.2330470, -1767.7669530},
                                   Waypt{1232.2330470, -1767.7669530},
                                   Waypt{1232.2330470, -1767.7669530},
                                   Waypt{1964.4660941, -2500.0000000},
                                   Waypt{4000.0000000, -2500.0000000}}, -1);
    plan.setCrossTrack(-2500.0000000);
    plan.setProperty(Plan::Property::IS_LIMIT);
    double ell0 = 449.7475;
    double lh0 = 2500.0;
    double lh_expect = 2500.0;
    double ellMax_expect = 2949.7475;

    QTest::newRow("Test 1") << plan << ell0 << lh0 << lh_expect << ellMax_expect;

    //Test 2
    plan.setPlan_skipCheck(QVector<Waypt>{Waypt{1500.0000000, -1500.0000000},
                                   Waypt{1500.0000000, -1500.0000000},
                                   Waypt{1500.0000000, -1500.0000000},
                                   Waypt{2121.3203436, -2121.3203436},
                                   Waypt{4000.0000000, -2121.3203436}}, -1);
    plan.setCrossTrack(-2121.3203436);
    plan.setProperty(Plan::Property::IS_LIMIT, false);

    ell0 = 381.6234;
    lh0 = 2500.0;
    lh_expect = 2375.7359;
    ellMax_expect = 2881.6234;

    QTest::newRow("Test 2") << plan << ell0 << lh0 << lh_expect << ellMax_expect;
}

//----------
void SMapHelperQTests::verify_determineArcLengthHorizon()
{
    QFETCH(MockPlan, plan);
    QFETCH(double, ell0);
    QFETCH(double, lh0);
    QFETCH(double, lh_expect);
    QFETCH(double, ellMax_expect);

    double lh, ellMax;
    SMapHelper::determineArcLengthHorizon(plan, ell0, lh0, lh, ellMax);
    QVERIFY(UtilHelper::compare(lh, lh_expect, 1e-3));
    QVERIFY(UtilHelper::compare(ellMax, ellMax_expect, 1e-3));
}

//----------
void SMapHelperQTests::verify_vol3_data()
{
    QTest::addColumn<QVector<double>>("x_vec");
    QTest::addColumn<QVector<double>>("L_vec");
    QTest::addColumn<QVector<double>>("x_lim");
    QTest::addColumn<double>("Umin");
    QTest::addColumn<double>("Umax");
    QTest::addColumn<double>("vol_expect");

    QVector<double> x_vec{}, L_vec{}, x_lim{};
    double Umin{}, Umax{}, vol_expect{};

    //Test 1
    x_vec = QVector<double>{-2500.0000, -2312.9297};
    L_vec = QVector<double>{2500.0000, 2500.0000};
    x_lim = QVector<double>{-2500.0000, -2312.9297};
    Umin = 7.7167;
    Umax = 15.4333;
    vol_expect = 37878735.1600;
    QTest::newRow("Test 1") << x_vec << L_vec << x_lim << Umin << Umax << vol_expect;
}

//----------
void SMapHelperQTests::verify_vol3()
{
    QFETCH(QVector<double>, x_vec);
    QFETCH(QVector<double>, L_vec);
    QFETCH(QVector<double>, x_lim);
    QFETCH(double, Umin);
    QFETCH(double, Umax);
    QFETCH(double, vol_expect);
    double vol = SMapHelper::sampling_vol3( x_vec.data(), L_vec.data(), x_lim.data(), Umin, Umax);
    //QVERIFY(UtilHelper::compare(vol, vol_expect, 1e-3)); //absolute value was different due to rounding effects
    double pct_diff = abs(vol - vol_expect)/vol_expect;
    QVERIFY(pct_diff < 1e-3);
}

//----------
void SMapHelperQTests::verify_vol4_data()
{
    QTest::addColumn<QVector<double>>("x_vec");
    QTest::addColumn<QVector<double>>("L_vec");
    QTest::addColumn<QVector<double>>("x_lim");
    QTest::addColumn<double>("Th");
    QTest::addColumn<double>("Umin");
    QTest::addColumn<double>("Umax");
    QTest::addColumn<double>("vol_expect");

    QVector<double> x_vec{}, L_vec{}, x_lim{};
    double Th{}, Umin{}, Umax{}, vol_expect{};

    //Test 1
    x_vec = QVector<double>{-2500.0000, -2312.9297};
    L_vec = QVector<double>{2500.0000, 2500.0000};
    x_lim = QVector<double>{-2500.0000, -2312.9297};
    Th = 375.0;
    Umin = 7.7167;
    Umax = 15.4333;
    vol_expect = 35999500.2754;
    QTest::newRow("Test 1") << x_vec << L_vec << x_lim << Th << Umin << Umax << vol_expect;
}

//----------
void SMapHelperQTests::verify_vol4()
{
    QFETCH(QVector<double>, x_vec);
    QFETCH(QVector<double>, L_vec);
    QFETCH(QVector<double>, x_lim);
    QFETCH(double, Th);
    QFETCH(double, Umin);
    QFETCH(double, Umax);
    QFETCH(double, vol_expect);

    double vol = SMapHelper::sampling_vol4( x_vec.data(), L_vec.data(), x_lim.data(), Th, Umin, Umax);
    //QVERIFY(UtilHelper::compare(vol, vol_expect, 1e-3)); //absolute value was different due to rounding effects
    double pct_diff = abs(vol - vol_expect)/vol_expect;
    QVERIFY(pct_diff < 1e-3);
}

//----------
void SMapHelperQTests::verify_create_data()
{
    QTest::addColumn<Plan>("planNominal");
    QTest::addColumn<double>("crossTrackHorizon");
    QTest::addColumn<VectorF>("posNE");
    QTest::addColumn<double>("Th");
    QTest::addColumn<double>("Lh");
    QTest::addColumn<double>("Umin");
    QTest::addColumn<double>("Umax");
    QTest::addColumn<int>("nPlan_expect");
    QTest::addColumn<int>("idxNominal_expect");
    QTest::addColumn<SMap>("sMap_expect");

    //set nominal plan
    Plan planNominal;
    planNominal.setPlan(QVector<Waypt>{Waypt{0.0, 0.0, 0.0, 0},
                                Waypt{1000.0, 1000.0, 0.0, 1},
                                Waypt{2000.0, 1000.0, 0.0, 2},
                                Waypt{3000.0, 0.0, 0.0, 3},
                                Waypt{4000.0, 0.0, 0.0, 4}},
                 0);
    planNominal.setProperty(Plan::Property::IS_NOMINAL);
    double crossTrackHorizon = 2500.0;
    VectorF posNE{2200, 0};
    double Th, Lh, Umin, Umax;
    Lh = 2500.0;
    Th = 375.0;
    Umin = 7.7167;
    Umax = 15.4333;

    int nPlan_expect = 7;
    int idxNominal_expect = 4;
    SMap sMap_expect;
    sMap_expect.append(SPlan{-2500, 2500, 0, 0});
    sMap_expect.append(SPlan{-2312.929715, 2500.000000, 0.069840, 0.053420});
    sMap_expect.append(SPlan{-2121.320344, 2375.735931, 0.137878, 0.106777});
    sMap_expect.append(SPlan{-1207.106781, 2300.000000, 0.436380, 0.350911});
    sMap_expect.append(SPlan{0.000000, 1800.000000, 0.740904, 0.633569});
    sMap_expect.append(SPlan{2414.213562, 800.000000, 0.996637, 0.992062});
    sMap_expect.append(SPlan{2500.000000, 820.101013, 1.000000, 1.000000});

    QTest::newRow("Test 1") << planNominal << crossTrackHorizon << posNE << Th << Lh << Umin << Umax << \
                               nPlan_expect << idxNominal_expect << sMap_expect;
}

//----------
void SMapHelperQTests::verify_create()
{
    QFETCH(Plan, planNominal);
    QFETCH(double, crossTrackHorizon);
    QFETCH(VectorF, posNE);
    QFETCH(double, Th);
    QFETCH(double, Lh);
    QFETCH(double, Umin);
    QFETCH(double, Umax);
    QFETCH(int, nPlan_expect);
    QFETCH(int, idxNominal_expect);
    QFETCH(SMap, sMap_expect);

    EllMap ellMap;
    bool res = ellMap.buildEllMap(planNominal, crossTrackHorizon);
    QVERIFY(res);

    RootData rootData;
    res = ellMap.getRootData(posNE, rootData);
    QVERIFY(res);

    SMap sMap = SMapHelper::create(ellMap,       //ellmap input
                    rootData,  //data relevant to current usv's position
                    Lh,                 //[m] desired arclength horizon
                    Th,                 //[s] desired time horizon
                    Umin,                //[m/s] min speed
                    Umax                //[m/s] max speed
                    );

    QCOMPARE(sMap.size(), nPlan_expect);
    QCOMPARE(sMap.idxNominal(), idxNominal_expect);

//    qInfo() << sMap;

    for(int i = 0; i < sMap.size(); ++i){
        QVERIFY(UtilHelper::compare(sMap.at(i).getCrosstrack(), sMap_expect.at(i).getCrosstrack(), 1e-3));
        QVERIFY(UtilHelper::compare(sMap.at(i).getLh(), sMap_expect.at(i).getLh(), 1e-3));
        QVERIFY(UtilHelper::compare(sMap.at(i).getVol_cum(), sMap_expect.at(i).getVol_cum(), 1e-3));
        QVERIFY(UtilHelper::compare(sMap.at(i).getArea_cum(), sMap_expect.at(i).getArea_cum(), 1e-3));
    }

}

