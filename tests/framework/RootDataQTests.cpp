#include "RootDataQTests.h"
#include <RrtPlannerLib/framework/RootData.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>


using namespace rrtplanner::framework;

//----------
RootDataQTests::RootDataQTests()
{

}

//----------
RootDataQTests::~RootDataQTests()
{
    cleanUp();
}

//----------
void RootDataQTests::setup()
{

}

//----------
void RootDataQTests::cleanUp()
{

}

//----------
void RootDataQTests::verify_constructor()
{
    VectorF posNE{2.3, -4.5};
    RootData rootData(posNE);
    QVERIFY(VectorFHelper::compare(rootData.posNE(), posNE, 1e-5));
}

//----------
void RootDataQTests::verify_setters_getters()
{
    VectorF posNE{-999, 888}; //usv position.

    //params of offset plan at usv position.
    double dx{45}; //[m] cross-track dist. wrt nominal plan.
    double ell{98}; //[m] length from start to current usv position.
    double L{123}; //[m] length of segment that usv position is on.
    double f_ell{0.99}; //fraction dl/L, where dl is the distance from start of current segment to usv position.

    //params for the polygon sector in which the root is inside.
    int planIdx{5}; //plan idx associated with the polygon sector that root is inside.
    int segIdx{23}; //segment idx associated with the polygon sector that root is inside.
    bool isInPoly{true}; //usv position is within the span of all the plans in EllMap.

    RootData rootData;
    rootData.setPosNE(posNE);
    rootData.setDx(dx);
    rootData.setEll(ell);
    rootData.setL(L);
    rootData.setF_ell(f_ell);
    rootData.setPlanIdx(planIdx);
    rootData.setSegIdx(segIdx);
    rootData.setIsInPoly(isInPoly);

    QVERIFY(VectorFHelper::compare(rootData.posNE(), posNE, 1e-5));
    QCOMPARE(rootData.dx(), dx);
    QCOMPARE(rootData.ell(), ell);
    QCOMPARE(rootData.L(), L);
    QCOMPARE(rootData.f_ell(), f_ell);
    QCOMPARE(rootData.planIdx(), planIdx);
    QCOMPARE(rootData.segIdx(), segIdx);
    QCOMPARE(rootData.isInPoly(), isInPoly);
}

