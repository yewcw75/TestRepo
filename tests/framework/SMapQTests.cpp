#include "SMapQTests.h"
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

class MockSMap: public SMap
{
public:
    MockSMap()
        :SMap(){}
    virtual ~MockSMap(){}
    void append(const SPlan& sPlan){SMap::append(sPlan);}
    void setIdxNominal(int idxNominal){SMap::setIdxNominal(idxNominal);}
    void setLh0(double lh0){SMap::setLh0(lh0);}
    void setTh0(double th0){SMap::setTh0(th0);}
    void setUmin(double umin){SMap::setUmin(umin);}
    void setUmax(double umax){SMap::setUmax(umax);}

};

//----------
SMapQTests::SMapQTests()
{

}

//----------
SMapQTests::~SMapQTests()
{
    cleanUp();
}

//----------
void SMapQTests::setup()
{

}

//----------
void SMapQTests::cleanUp()
{

}

//----------
void SMapQTests::verify_setEllMap_data()
{
    QTest::addColumn<EllMap>("ellMap");
    QTest::addColumn<double>("Th");
    QTest::addColumn<double>("Lh");
    QTest::addColumn<double>("Umin");
    QTest::addColumn<double>("Umax");

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
    EllMap ellMap;
    bool buildOk = ellMap.buildEllMap(planNominal, crossTrackHorizon);
    QVERIFY(buildOk);

    double Th, Lh, Umin, Umax;
    Lh = 2500.0;
    Th = 375.0;
    Umin = 7.7167;
    Umax = 15.4333;

    QTest::newRow("Test 1") << ellMap << Th << Lh << Umin << Umax;
}

//----------
void SMapQTests::verify_setEllMap()
{
    QFETCH(EllMap, ellMap);
    QFETCH(double, Th);
    QFETCH(double, Lh);
    QFETCH(double, Umin);
    QFETCH(double, Umax);

    SMap sMap;
    sMap.setEllMap(ellMap, Lh, Th, Umin, Umax);

    QVERIFY(UtilHelper::compare(sMap.lh0(), Lh, 1e-3));
    QVERIFY(UtilHelper::compare(sMap.th0(), Th, 1e-3));
    QVERIFY(UtilHelper::compare(sMap.umin(), Umin, 1e-3));
    QVERIFY(UtilHelper::compare(sMap.umax(), Umax, 1e-3));
    QVERIFY(UtilHelper::compare(sMap.ellMap(), ellMap, 1e-3));
}

//----------
void SMapQTests::verify_reset_data()
{
    QTest::addColumn<SMap>("sMap");
    QTest::addColumn<VectorF>("posNE");
    QTest::addColumn<SMap>("sMap_expect");
    QTest::addColumn<int>("nPlan_expect");
    QTest::addColumn<int>("idxNominal_expect");

    //Test 1
    Plan planNominal;
    planNominal.setPlan(QVector<Waypt>{Waypt{0.0, 0.0, 0.0, 0},
                                Waypt{1000.0, 1000.0, 0.0, 1},
                                Waypt{2000.0, 1000.0, 0.0, 2},
                                Waypt{3000.0, 0.0, 0.0, 3},
                                Waypt{4000.0, 0.0, 0.0, 4}},
                 0);
    planNominal.setProperty(Plan::Property::IS_NOMINAL);
    double crossTrackHorizon = 2500.0;
    EllMap ellMap;
    bool buildOk = ellMap.buildEllMap(planNominal, crossTrackHorizon);
    QVERIFY(buildOk);

    double Th, Lh, Umin, Umax;
    Lh = 2500.0;
    Th = 375.0;
    Umin = 7.7167;
    Umax = 15.4333;
    SMap sMap;
    sMap.setEllMap(ellMap, Lh, Th, Umin, Umax);

    VectorF posNE{2200, 0};

    MockSMap sMap_expect;
    sMap_expect.append(SPlan{-2500, 2500, 0, 0});
    sMap_expect.append(SPlan{-2312.929715, 2500.000000, 0.069840, 0.053420});
    sMap_expect.append(SPlan{-2121.320344, 2375.735931, 0.137878, 0.106777});
    sMap_expect.append(SPlan{-1207.106781, 2300.000000, 0.436380, 0.350911});
    sMap_expect.append(SPlan{0.000000, 1800.000000, 0.740904, 0.633569});
    sMap_expect.append(SPlan{2414.213562, 800.000000, 0.996637, 0.992062});
    sMap_expect.append(SPlan{2500.000000, 820.101013, 1.000000, 1.000000});
    sMap_expect.setIdxNominal(4);
    sMap_expect.setTh0(Th);
    sMap_expect.setLh0(Lh);
    sMap_expect.setUmin(Umin);
    sMap_expect.setUmax(Umax);

    QTest::newRow("Test 1") << sMap << posNE << (SMap)sMap_expect;
}

//----------
void SMapQTests::verify_reset()
{
    QFETCH(SMap, sMap);
    QFETCH(VectorF, posNE);
    QFETCH(SMap, sMap_expect);

    bool res = sMap.reset(posNE);
    QVERIFY(res);
    QVERIFY(UtilHelper::compare(sMap, sMap_expect, 1e-3));
}

