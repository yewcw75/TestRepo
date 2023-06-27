#include "WayptQTests.h"
#include <QtTest/QtTest>

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
void WayptQTests::verify(Waypt waypt, float northing, float easting, double lon0, int id)
{
    QCOMPARE(waypt.northing(), northing);
    QCOMPARE(waypt.easting(), easting);
    QCOMPARE(waypt.lon0_deg(), lon0);
    QCOMPARE(waypt.id(), id);

    QVERIFY(waypt.getFieldFlags().testFlag(Waypt::Field::NORTHING));
    QVERIFY(waypt.getFieldFlags().testFlag(Waypt::Field::EASTING));
    QVERIFY(waypt.getFieldFlags().testFlag(Waypt::Field::LON0));
    QVERIFY(waypt.getFieldFlags().testFlag(Waypt::Field::ID));
    return;
}

//----------
void WayptQTests::verify_set_params_data()
{
    QTest::addColumn<float>("northing");
    QTest::addColumn<float>("easting");
    QTest::addColumn<double>("lon0");
    QTest::addColumn<int>("id");

    QTest::newRow("Test 1") << (float)-12345.54321 << (float)98765.5678 << (double)887766 << (int)-9999;
    QTest::newRow("Test 2") << (float)12345.54321 << (float)-98765.5678 << (double)-887766 << (int)9999;
    QTest::newRow("Test 3") << (float)1.2345 << (float)103.2233 << (double)-104.0 << (int)1;

    return;
}

//----------
void WayptQTests::verify_set_params()
{
    setup();
    QFETCH(float, northing);
    QFETCH(float, easting);
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
    Coord_NE coord{northing, easting};
    Waypt waypt3;
    waypt3.set(coord, lon0, id);
    verify(waypt3, northing, easting, lon0, id);

    //test constructor
    Waypt waypt4(coord, lon0, id);
    verify(waypt4, northing, easting, lon0, id);

    return;
}

//----------
void WayptQTests::verify_setters_data()
{
    QTest::addColumn<float>("northing");
    QTest::addColumn<float>("easting");
    QTest::addColumn<double>("lon0");
    QTest::addColumn<int>("id");

    QTest::newRow("Test 1") << (float)-12345.54321 << (float)98765.5678 << (double)887766 << (int)-9999;
    QTest::newRow("Test 2") << (float)12345.54321 << (float)-98765.5678 << (double)-887766 << (int)9999;
    QTest::newRow("Test 3") << (float)1.2345 << (float)103.2233 << (double)-104.0 << (int)1;

    return;
}

//----------
void WayptQTests::verify_setters()
{
    setup();
    QFETCH(float, northing);
    QFETCH(float, easting);
    QFETCH(double, lon0);
    QFETCH(int, id);

    Waypt waypt;
    QVERIFY(waypt.getFieldFlags().testFlag(Waypt::Field::NONE));
    waypt.setNorthing(northing);
    waypt.setEasting(easting);
    waypt.setLon0(lon0);
    waypt.setId(id);
    verify(waypt, northing, easting, lon0, id);

    Coord_NE coord = waypt.coord_const_ref();
    QCOMPARE(coord.get<IDX_NORTHING>(), northing);
    QCOMPARE(coord.get<IDX_EASTING>(), easting);

    return;
}

//----------
void WayptQTests::verify_copy_data()
{
    QTest::addColumn<float>("northing");
    QTest::addColumn<float>("easting");
    QTest::addColumn<double>("lon0");
    QTest::addColumn<int>("id");

    QTest::newRow("Test 1") << (float)-12345.54321 << (float)98765.5678 << (double)887766 << (int)-9999;
    QTest::newRow("Test 2") << (float)12345.54321 << (float)-98765.5678 << (double)-887766 << (int)9999;
    QTest::newRow("Test 3") << (float)1.2345 << (float)103.2233 << (double)-104.0 << (int)1;

    return;
}

//----------
void WayptQTests::verify_copy()
{
    setup();
    QFETCH(float, northing);
    QFETCH(float, easting);
    QFETCH(double, lon0);
    QFETCH(int, id);

    Waypt waypt;
    waypt.setNorthing(northing);
    waypt.setEasting(easting);
    waypt.setLon0(lon0);
    waypt.setId(id);

    Waypt wayptCopy(waypt);
    verify(wayptCopy, northing, easting, lon0, id);
    QCOMPARE(wayptCopy.getFieldFlags(), waypt.getFieldFlags());

    Waypt wayptCopy2 = waypt;
    verify(wayptCopy2, northing, easting, lon0, id);
    QCOMPARE(wayptCopy2.getFieldFlags(), waypt.getFieldFlags());
}

