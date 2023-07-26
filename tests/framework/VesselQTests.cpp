#include "VesselQTests.h"
#include <RrtPlannerLib/framework/Vessel.h>
#include <RrtPlannerLib/framework/VesRectangle.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>
#include <math.h>


using namespace rrtplanner::framework;

//----------
VesselQTests::VesselQTests()
{

}

//----------
VesselQTests::~VesselQTests()
{
    cleanUp();
}

//----------
void VesselQTests::setup()
{

}

//----------
void VesselQTests::cleanUp()
{

}

//----------
void VesselQTests::verify_constructor_data()
{
    QTest::addColumn<VectorF>("posNE");
    QTest::addColumn<double>("lon0_deg");
    QTest::addColumn<VectorF>("velNE");
    QTest::addColumn<double>("hdg");
    QTest::addColumn<VesRectangle>("vesRectangle");

    double rotation = 30.0;
    VectorF offset{433, 566};
    double length = 99.9;
    double width = 33.44;
    VesRectangle r1(length, width);
    r1.setRotation(rotation);
    r1.setOffset(offset);

    QTest::newRow("Test 1") << VectorF{12,23} << 3.44 << VectorF{55.66, 66.77} << 43.2 << r1;
}

//----------
void VesselQTests::verify_constructor()
{
    QFETCH(VectorF, posNE);
    QFETCH(double, lon0_deg);
    QFETCH(VectorF, velNE);
    QFETCH(double, hdg);
    QFETCH(VesRectangle, vesRectangle);

    Vessel vessel(posNE, lon0_deg, velNE, hdg);
    vessel.setVesShape(QSharedPointer<VesShape>(vesRectangle.clone()));

    QVERIFY(VectorFHelper::compare(vessel.posNE(), posNE, 1e-3));
    QVERIFY(VectorFHelper::compare(vessel.velNE(), velNE, 1e-3));
    QCOMPARE(vessel.hdg_deg(), hdg);
    QCOMPARE(vessel.lon0_deg(), lon0_deg);

    VesRectangle* r = dynamic_cast<VesRectangle*>(vessel.vesShape().data());
    QCOMPARE(r->length(), vesRectangle.length());
    QCOMPARE(r->width(), vesRectangle.width());
    QCOMPARE(r->rotation(), vesRectangle.rotation());
    double diff = VectorFHelper::norm2(VectorFHelper::subtract_vector(r->offset(), vesRectangle.offset()));
    QVERIFY(UtilHelper::compare(diff, 0.0, 1e-3));
}
