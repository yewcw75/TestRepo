#include "VesRectangleQTests.h"
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>
#include <math.h>


using namespace rrtplanner::framework;

//----------
VesRectangleQTests::VesRectangleQTests()
{

}

//----------
VesRectangleQTests::~VesRectangleQTests()
{
    cleanUp();
}

//----------
void VesRectangleQTests::setup()
{

}

//----------
void VesRectangleQTests::cleanUp()
{

}

//----------
void VesRectangleQTests::verify_constructor()
{
    double rotation = 30.0;
    VectorF offset{433, 566};
    double length = 99.9;
    double width = 33.44;
    VesRectangle r1(length, width);
    r1.setRotation(rotation);
    r1.setOffset(offset);

    QCOMPARE(r1.length(), length);
    QCOMPARE(r1.width(), width);
    QCOMPARE(r1.rotation(), rotation);
    double diff = VectorFHelper::norm2(VectorFHelper::subtract_vector(r1.offset(), offset));
    QVERIFY(UtilHelper::compare(diff, 0.0, 1e-3));

    VesRectangle r2;
    r2.setDimensions(length, width);
    QCOMPARE(r2.length(), length);
    QCOMPARE(r2.width(), width);
}

//----------
void VesRectangleQTests::verify_getPolygon()
{
    double rotation = 30.0;
    double rotation_rad = rotation/180.0*M_PI;
    VectorF offset{433, 566};
    double length = 99.9;
    double width = 33.44;
    VesRectangle r1(length, width);
    r1.setRotation(rotation);
    r1.setOffset(offset);

    QList<VectorF> poly = r1.polygon();
    QCOMPARE(poly.size(), 4);

    //verify centroid
    double uc{0}, vc{0}; //centroid
    for(int i = 0; i < 4; ++i){
        uc += poly.at(i).at(0);
        vc += poly.at(i).at(1);
        qInfo() << poly[i];
    }
    uc = uc/4.0; vc = vc/4.0;
    QVERIFY(UtilHelper::compare(uc, offset.at(0)));
    QVERIFY(UtilHelper::compare(vc, offset.at(1)));

    //verify width
    double w = VectorFHelper::norm2(VectorFHelper::subtract_vector(poly.at(0), poly.at(3)));
    QVERIFY(UtilHelper::compare(w, width));

    //verify length
    double l = VectorFHelper::norm2(VectorFHelper::subtract_vector(poly.at(2), poly.at(3)));
    QVERIFY(UtilHelper::compare(l, length));


    //verify angle
    VectorF lvec = VectorFHelper::subtract_vector(poly.at(3), poly.at(2));
    double rot_rad = atan2(lvec.at(1), lvec.at(0));
    QVERIFY(UtilHelper::compare(rot_rad, rotation_rad));

}

//----------
void VesRectangleQTests::verify_clone()
{
    double rotation = 30.0;
    VectorF offset{433, 566};
    double length = 99.9;
    double width = 33.44;
    VesRectangle* r1 = new VesRectangle(length, width);
    r1->setRotation(rotation);
    r1->setOffset(offset);

    VesShape* s2 = r1->clone();
    QCOMPARE(s2->type(), VesShape::Type::RECTANGLE);

    VesRectangle* r2 = dynamic_cast<VesRectangle*>(s2);
    QVERIFY(r2 != nullptr);

    QCOMPARE(r2->length(), length);
    QCOMPARE(r2->width(), width);
    QCOMPARE(r2->rotation(), rotation);
    QCOMPARE(r2->offset().at(0), offset.at(0));
    QCOMPARE(r2->offset().at(1), offset.at(1));

    delete r1;
    delete s2;
}
