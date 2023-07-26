#include "SegmentQTests.h"
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>

using namespace rrtplanner::framework;

//----------
SegmentQTests::SegmentQTests()
{

}

//----------
SegmentQTests::~SegmentQTests()
{
    cleanUp();
}

//----------
void SegmentQTests::setup()
{

}

//----------
void SegmentQTests::cleanUp()
{

}

//----------
void SegmentQTests::verify(const Segment& segment,
            int id,
            double northing_wayptPrev,
            double easting_wayptPrev,
            double northing_wayptNext,
            double easting_wayptNext,
            double northing_res_tVec,
            double easting_res_tVec,
            double northing_res_nVec,
            double easting_res_nVec,
            double segLength_res)
{
    VectorF tVec = segment.tVec();
    VectorF nVec = segment.nVec();
    double length = segment.length();

    //check waypts are set correctly
    QCOMPARE(segment.id(), id);
    QCOMPARE(segment.wayptPrev().northing(), northing_wayptPrev);
    QCOMPARE(segment.wayptPrev().easting(), easting_wayptPrev);
    QCOMPARE(segment.wayptNext().northing(), northing_wayptNext);
    QCOMPARE(segment.wayptNext().easting(), easting_wayptNext);

    //check derived attributes are correct
    //qInfo() << "tVec[IDX_NORTHING]: " << QString::number(tVec[IDX_NORTHING], 'f', 10) << " vs northing_res_tVec: " <<  QString::number(northing_res_tVec, 'f', 10);
    QVERIFY(UtilHelper::compare(tVec[IDX_NORTHING], northing_res_tVec));
    QVERIFY(UtilHelper::compare(tVec[IDX_EASTING], easting_res_tVec));
    QVERIFY(UtilHelper::compare(nVec[IDX_NORTHING], northing_res_nVec));
    QVERIFY(UtilHelper::compare(nVec[IDX_EASTING], easting_res_nVec));
    QVERIFY(UtilHelper::compare(length, segLength_res));

    
}

//----------
void SegmentQTests::verify_set_segment_data()
{
    QTest::addColumn<int>("id");
    QTest::addColumn<double>("northing_wayptPrev");
    QTest::addColumn<double>("easting_wayptPrev");
    QTest::addColumn<double>("northing_wayptNext");
    QTest::addColumn<double>("easting_wayptNext");
    QTest::addColumn<double>("northing_res_tVec");
    QTest::addColumn<double>("easting_res_tVec");
    QTest::addColumn<double>("northing_res_nVec");
    QTest::addColumn<double>("easting_res_nVec");
    QTest::addColumn<double>("segLength_res");

    QTest::newRow("Test 1") << (int) -99 << (double)10 << (double)30 << (double)100 << (double)200 \
                               << (double)0.467888 << (double)0.883788 << (double)-0.883788 << (double)0.467888 << (double)192.353841;
      QTest::newRow("Test 2") << (int) 99 << (double)10 << (double)0 << (double)100 << (double)-200 \
                               << (double)0.410365 << (double)-0.911922 << (double)0.911922 << (double)0.410365 << (double)219.317122;
    QTest::newRow("Test 3") << (int) 5678499 << (double)0 << (double)-10 << (double)-100 << (double)-200 \
                               << (double)-0.465746 << (double)-0.884918 << (double)0.884918 << (double)-0.465746 << (double)214.709106;
    QTest::newRow("Test 4") << (int) -9283499 << (double)0 << (double)-10 << (double)-100 << (double)200 \
                               << (double)-0.429934 << (double)0.902861 << (double)-0.902861 << (double)-0.429934 << (double)232.594067;
    
}

//----------
void SegmentQTests::verify_set_segment()
{
    setup();

    QFETCH(int, id);
    QFETCH(double, northing_wayptPrev);
    QFETCH(double, easting_wayptPrev);
    QFETCH(double, northing_wayptNext);
    QFETCH(double, easting_wayptNext);
    QFETCH(double, northing_res_tVec);
    QFETCH(double, easting_res_tVec);
    QFETCH(double, northing_res_nVec);
    QFETCH(double, easting_res_nVec);
    QFETCH(double, segLength_res);

    //verify set segment
    Waypt wayptPrev(northing_wayptPrev, easting_wayptPrev);
    Waypt wayptNext(northing_wayptNext, easting_wayptNext);
    Segment segment1;
    segment1.setWayptPrev(wayptPrev);
    segment1.setWayptNext(wayptNext);
    segment1.setId(id);
    segment1.setSegmentAttributes();

    verify(segment1, id, northing_wayptPrev, easting_wayptPrev, northing_wayptNext, easting_wayptNext, \
                northing_res_tVec, easting_res_tVec, northing_res_nVec, easting_res_nVec, segLength_res);

    //verify set method
    Segment segment2;
    segment2.set(wayptPrev, wayptNext, id);
    segment2.setSegmentAttributes();
    verify(segment2, id, northing_wayptPrev, easting_wayptPrev, northing_wayptNext, easting_wayptNext, \
                northing_res_tVec, easting_res_tVec, northing_res_nVec, easting_res_nVec, segLength_res);

    //verify constructor
    Segment segment3(wayptPrev, wayptNext, id);
    segment3.setSegmentAttributes();
    verify(segment3, id, northing_wayptPrev, easting_wayptPrev, northing_wayptNext, easting_wayptNext, \
                northing_res_tVec, easting_res_tVec, northing_res_nVec, easting_res_nVec, segLength_res);
}

//----------
void SegmentQTests::verify_set_bisector()
{
    Segment segment0(Waypt(0, 0), Waypt(1000, 1000), 0);
    segment0.setSegmentAttributes();
    Segment segment1(Waypt(1000, 1000), Waypt(2000, 1000), 1);
    segment1.setSegmentAttributes();
    Segment segment2(Waypt(2000, 1000), Waypt(3000, 0), 2);
    segment2.setSegmentAttributes();

    segment1.setbVecPrev(segment0);

    segment1.setbVecNext(segment2);

    VectorF bVecPrev = segment1.bVecPrev();
    QVERIFY(UtilHelper::compare(bVecPrev[IDX_NORTHING], (double)(-0.38268343)));
    QVERIFY(UtilHelper::compare(bVecPrev[IDX_EASTING], (double)0.92387953));

    VectorF bVecNext = segment1.bVecNext();
    QVERIFY(UtilHelper::compare(bVecNext[IDX_NORTHING], (double)0.38268343));
    QVERIFY(UtilHelper::compare(bVecNext[IDX_EASTING], (double)0.92387953));
}

//----------
void SegmentQTests::verify_set_bisector_2()
{
    Segment segment(Waypt(0, 0), Waypt(1000, 1000), 0);
    segment.setSegmentAttributes();
    VectorF bVecPrev{0.38268343, 0.92387953};
    VectorF bVecNext{-0.44721360, 0.89442719};

    segment.setbVecPrev(bVecPrev);
    segment.setbVecNext(bVecNext);

    VectorF bVecPrev_res = segment.bVecPrev();
    QVERIFY(UtilHelper::compare(bVecPrev_res[IDX_NORTHING], (double)0.38268343));
    QVERIFY(UtilHelper::compare(bVecPrev_res[IDX_EASTING], (double)0.92387953));

    VectorF bVecNext_res = segment.bVecNext();
    QVERIFY(UtilHelper::compare(bVecNext_res[IDX_NORTHING], (double)(-0.44721360)));
    QVERIFY(UtilHelper::compare(bVecNext_res[IDX_EASTING], (double)0.89442719));

}
