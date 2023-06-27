#include "SegmentQTests.h"
#include <QtTest/QtTest>
#include <QtGlobal>

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
            float northing_nodePrev,
            float easting_nodePrev,
            float northing_nodeNext,
            float easting_nodeNext,
            float northing_res_tVec,
            float easting_res_tVec,
            float northing_res_nVec,
            float easting_res_nVec,
            float segLength_res)
{
    Vector_NE tVec = segment.tVec();
    Vector_NE nVec = segment.nVec();
    float length = segment.length();

    //check nodes are set correctly
    QCOMPARE(segment.id(), id);
    QCOMPARE(segment.nodePrev().northing(), northing_nodePrev);
    QCOMPARE(segment.nodePrev().easting(), easting_nodePrev);
    QCOMPARE(segment.nodeNext().northing(), northing_nodeNext);
    QCOMPARE(segment.nodeNext().easting(), easting_nodeNext);

    //check derived attributes are correct
    QVERIFY(qFuzzyCompare(tVec.get<IDX_NORTHING>(), northing_res_tVec));
    QVERIFY(qFuzzyCompare(tVec.get<IDX_EASTING>(), easting_res_tVec));
    QVERIFY(qFuzzyCompare(nVec.get<IDX_NORTHING>(), northing_res_nVec));
    QVERIFY(qFuzzyCompare(nVec.get<IDX_EASTING>(), easting_res_nVec));
    QVERIFY(qFuzzyCompare(length, segLength_res));

    //check field flags
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::ID));
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::NODE_PREV));
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::NODE_NEXT));
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::TVEC));
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::NVEC));
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::LENGTH));

    return;
}

//----------
void SegmentQTests::verify_set_segment_data()
{
    QTest::addColumn<int>("id");
    QTest::addColumn<float>("northing_nodePrev");
    QTest::addColumn<float>("easting_nodePrev");
    QTest::addColumn<float>("northing_nodeNext");
    QTest::addColumn<float>("easting_nodeNext");
    QTest::addColumn<float>("northing_res_tVec");
    QTest::addColumn<float>("easting_res_tVec");
    QTest::addColumn<float>("northing_res_nVec");
    QTest::addColumn<float>("easting_res_nVec");
    QTest::addColumn<float>("segLength_res");

    QTest::newRow("Test 1") << (int) -99 << (float)10 << (float)30 << (float)100 << (float)200 \
                               << (float)0.467888 << (float)0.883788 << (float)-0.883788 << (float)0.467888 << (float)192.353841;
      QTest::newRow("Test 2") << (int) 99 << (float)10 << (float)0 << (float)100 << (float)-200 \
                               << (float)0.410365 << (float)-0.911922 << (float)0.911922 << (float)0.410365 << (float)219.317122;
    QTest::newRow("Test 3") << (int) 5678499 << (float)0 << (float)-10 << (float)-100 << (float)-200 \
                               << (float)-0.465746 << (float)-0.884918 << (float)0.884918 << (float)-0.465746 << (float)214.709106;
    QTest::newRow("Test 4") << (int) -9283499 << (float)0 << (float)-10 << (float)-100 << (float)200 \
                               << (float)-0.429934 << (float)0.902861 << (float)-0.902861 << (float)-0.429934 << (float)232.594067;
    return;
}

//----------
void SegmentQTests::verify_set_segment()
{
    setup();

    QFETCH(int, id);
    QFETCH(float, northing_nodePrev);
    QFETCH(float, easting_nodePrev);
    QFETCH(float, northing_nodeNext);
    QFETCH(float, easting_nodeNext);
    QFETCH(float, northing_res_tVec);
    QFETCH(float, easting_res_tVec);
    QFETCH(float, northing_res_nVec);
    QFETCH(float, easting_res_nVec);
    QFETCH(float, segLength_res);

    //verify set segment
    Waypt nodePrev(northing_nodePrev, easting_nodePrev);
    Waypt nodeNext(northing_nodeNext, easting_nodeNext);
    Segment segment1;
    QVERIFY(segment1.getFieldFlags().testFlag(Segment::Field::NONE));
    segment1.setNodePrev(nodePrev);
    segment1.setNodeNext(nodeNext);
    segment1.setId(id);

    verify(segment1, id, northing_nodePrev, easting_nodePrev, northing_nodeNext, easting_nodeNext, \
                northing_res_tVec, easting_res_tVec, northing_res_nVec, easting_res_nVec, segLength_res);

    //verify set method
    Segment segment2;
    segment2.set(nodePrev, nodeNext, id);
    verify(segment2, id, northing_nodePrev, easting_nodePrev, northing_nodeNext, easting_nodeNext, \
                northing_res_tVec, easting_res_tVec, northing_res_nVec, easting_res_nVec, segLength_res);

    //verify constructor
    Segment segment3(nodePrev, nodeNext, id);
    verify(segment3, id, northing_nodePrev, easting_nodePrev, northing_nodeNext, easting_nodeNext, \
                northing_res_tVec, easting_res_tVec, northing_res_nVec, easting_res_nVec, segLength_res);
}

//----------
void SegmentQTests::verify_set_bisector()
{
    Segment segment0(Waypt(0, 0), Waypt(1000, 1000), 0);
    Segment segment1(Waypt(1000, 1000), Waypt(2000, 1000), 1);
    Segment segment2(Waypt(2000, 1000), Waypt(3000, 0), 2);

    QVERIFY(segment1.getFieldFlags().testFlag(Segment::Field::BVEC_PREV) == false);
    QVERIFY(segment1.getFieldFlags().testFlag(Segment::Field::BVEC_NEXT) == false);

    segment1.setbVecPrev(segment0);
    QVERIFY(segment1.getFieldFlags().testFlag(Segment::Field::BVEC_PREV));

    segment1.setbVecNext(segment2);
    QVERIFY(segment1.getFieldFlags().testFlag(Segment::Field::BVEC_NEXT));

    Vector_NE bVecPrev = segment1.bVecPrev();
    QVERIFY(qFuzzyCompare(bVecPrev.get<IDX_NORTHING>(), (float)(-0.38268343)));
    QVERIFY(qFuzzyCompare(bVecPrev.get<IDX_EASTING>(), (float)0.92387953));

    Vector_NE bVecNext = segment1.bVecNext();
    QVERIFY(qFuzzyCompare(bVecNext.get<IDX_NORTHING>(), (float)0.38268343));
    QVERIFY(qFuzzyCompare(bVecNext.get<IDX_EASTING>(), (float)0.92387953));
}

//----------
void SegmentQTests::verify_set_bisector_2()
{
    Segment segment(Waypt(0, 0), Waypt(1000, 1000), 0);
    Vector_NE bVecPrev(0.38268343, 0.92387953);
    Vector_NE bVecNext(-0.44721360, 0.89442719);

    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::BVEC_PREV) == false);
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::BVEC_NEXT) == false);

    segment.setbVecPrev(bVecPrev);
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::BVEC_PREV));
    segment.setbVecNext(bVecNext);
    QVERIFY(segment.getFieldFlags().testFlag(Segment::Field::BVEC_NEXT));

    Vector_NE bVecPrev_res = segment.bVecPrev();
    QVERIFY(qFuzzyCompare(bVecPrev_res.get<IDX_NORTHING>(), (float)0.38268343));
    QVERIFY(qFuzzyCompare(bVecPrev_res.get<IDX_EASTING>(), (float)0.92387953));

    Vector_NE bVecNext_res = segment.bVecNext();
    QVERIFY(qFuzzyCompare(bVecNext_res.get<IDX_NORTHING>(), (float)(-0.44721360)));
    QVERIFY(qFuzzyCompare(bVecNext_res.get<IDX_EASTING>(), (float)0.89442719));

}
