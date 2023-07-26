#include "WayptQTests.h"
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <QtTest/QtTest>

using namespace rrtplanner::framework;

//----------
WayptQTests::WayptQTests()
{

}

//----------
WayptQTests::~WayptQTests()
{
    cleanUp();
}

//----------
void WayptQTests::setup()
{

}

//----------
void WayptQTests::cleanUp()
{

}

//----------
void WayptQTests::verify(Waypt waypt, double northing, double easting, double lon0, int id)
{
    QCOMPARE(waypt.northing(), northing);
    QCOMPARE(waypt.easting(), easting);
    QCOMPARE(waypt.lon0_deg(), lon0);
    QCOMPARE(waypt.id(), id);
    
}

//----------
void WayptQTests::verify_set_params_data()
{
    QTest::addColumn<double>("northing");
    QTest::addColumn<double>("easting");
    QTest::addColumn<double>("lon0");
    QTest::addColumn<int>("id");

    QTest::newRow("Test 1") << (double)-12345.54321 << (double)98765.5678 << (double)887766 << (int)-9999;
    QTest::newRow("Test 2") << (double)12345.54321 << (double)-98765.5678 << (double)-887766 << (int)9999;
    QTest::newRow("Test 3") << (double)1.2345 << (double)103.2233 << (double)-104.0 << (int)1;

    
}

//----------
void WayptQTests::verify_set_params()
{
    setup();
    QFETCH(double, northing);
    QFETCH(double, easting);
    QFETCH(double, lon0);
    QFETCH(int, id);

    //test setter
    Waypt waypt;
    waypt.set(northing, easting, lon0, id);
    verify(waypt, northing, easting, lon0, id);

    //test constructor
    Waypt waypt2(northing, easting, lon0, id);
    verify(waypt2, northing, easting, lon0, id);

    //test setter
    VectorF coord{northing, easting};
    Waypt waypt3;
    waypt3.set(coord, lon0, id);
    verify(waypt3, northing, easting, lon0, id);

    //test constructor
    Waypt waypt4(coord, lon0, id);
    verify(waypt4, northing, easting, lon0, id);

    
}

//----------
void WayptQTests::verify_setters_data()
{
    QTest::addColumn<double>("northing");
    QTest::addColumn<double>("easting");
    QTest::addColumn<double>("lon0");
    QTest::addColumn<int>("id");

    QTest::newRow("Test 1") << (double)-12345.54321 << (double)98765.5678 << (double)887766 << (int)-9999;
    QTest::newRow("Test 2") << (double)12345.54321 << (double)-98765.5678 << (double)-887766 << (int)9999;
    QTest::newRow("Test 3") << (double)1.2345 << (double)103.2233 << (double)-104.0 << (int)1;

    
}

//----------
void WayptQTests::verify_setters()
{
    setup();
    QFETCH(double, northing);
    QFETCH(double, easting);
    QFETCH(double, lon0);
    QFETCH(int, id);

    Waypt waypt;
    waypt.setNorthing(northing);
    waypt.setEasting(easting);
    waypt.setLon0(lon0);
    waypt.setId(id);
    verify(waypt, northing, easting, lon0, id);

    VectorF coord = waypt.coord_const_ref();
    QCOMPARE(coord[IDX_NORTHING], northing);
    QCOMPARE(coord[IDX_EASTING], easting);

    
}

//----------
void WayptQTests::verify_copy_data()
{
    QTest::addColumn<double>("northing");
    QTest::addColumn<double>("easting");
    QTest::addColumn<double>("lon0");
    QTest::addColumn<int>("id");

    QTest::newRow("Test 1") << (double)-12345.54321 << (double)98765.5678 << (double)887766 << (int)-9999;
    QTest::newRow("Test 2") << (double)12345.54321 << (double)-98765.5678 << (double)-887766 << (int)9999;
    QTest::newRow("Test 3") << (double)1.2345 << (double)103.2233 << (double)-104.0 << (int)1;

    
}

//----------
void WayptQTests::verify_copy()
{
    setup();
    QFETCH(double, northing);
    QFETCH(double, easting);
    QFETCH(double, lon0);
    QFETCH(int, id);

    Waypt waypt;
    waypt.setNorthing(northing);
    waypt.setEasting(easting);
    waypt.setLon0(lon0);
    waypt.setId(id);

    Waypt wayptCopy(waypt);
    verify(wayptCopy, northing, easting, lon0, id);

    Waypt wayptCopy2 = waypt;
    verify(wayptCopy2, northing, easting, lon0, id);
}

