#include "PlanQTests.h"
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>

//----------
PlanQTests::PlanQTests()
{

}

//----------
PlanQTests::~PlanQTests()
{
    cleanUp();
}

//----------
void PlanQTests::setup()
{

}

//----------
void PlanQTests::cleanUp()
{

}

//----------
void PlanQTests:: verifySame(const Waypt& waypt1, const Waypt& waypt2)
{
    QCOMPARE(waypt1.id(), waypt2.id());
    QCOMPARE(waypt1.northing(), waypt2.northing());
    QCOMPARE(waypt1.easting(), waypt2.easting());
    QCOMPARE(waypt1.lon0_deg(), waypt2.lon0_deg());
    QCOMPARE(waypt1.getFieldFlags(), waypt2.getFieldFlags());
    return;
}

//----------
void PlanQTests::verify_set_plan_data()
{
    QTest::addColumn<int>("id");
    QTest::addColumn<QVector<Waypt>>("wayptList");
    QTest::addColumn<float>("planLength");
    QTest::addColumn<float>("crossTrack");
    QTest::addColumn<int>("propertyFlagsVal");
    QTest::addColumn<bool>("setOk");

    QVector<Waypt> wayptList;
    wayptList.append(Waypt(0, 0, 103, 0));
    wayptList.append(Waypt(1000, 1000, 103, 1));
    wayptList.append(Waypt(2000, 1000, 103, 2));
    wayptList.append(Waypt(3000, 0, 103, 3));
    wayptList.append(Waypt(4000, 0, 103, 3));

    Plan::PropertyFlags propertyFlags = Plan::Property::IS_LIMIT;
    QTest::newRow("Test 1") << (int) 1 << (QVector<Waypt>)wayptList << \
                               (float)4828.42712475 << (float)10.0 << (int)propertyFlags << (bool)true;

    QVector<Waypt> wayptList2;
    propertyFlags = Plan::Property::IS_NOMINAL;
    wayptList2.append(Waypt(0, 0, 103, 0));
    QTest::newRow("Test 2") << (int) 1 << (QVector<Waypt>)wayptList2 << \
                               (float)0.0 << (float)-10.0 << (int)propertyFlags << (bool)false;

    QVector<Waypt> wayptList3;
    wayptList3.append(Waypt(0, 0, 103, 0));
    wayptList3.append(Waypt(1000, 1000, 103, 1));
    wayptList3.append(Waypt(950, 1500, 103, 1)); //go back a little
    wayptList3.append(Waypt(2000, 1500, 103, 2));
    wayptList3.append(Waypt(3000, 0, 103, 3));
    wayptList3.append(Waypt(4000, 0, 103, 3));
    propertyFlags = Plan::Property::NONE;
    QTest::newRow("Test 3") << (int) 1 << (QVector<Waypt>)wayptList3 << \
                               (float)0.0 /*does not matter, expected not used*/ << (float)10.0 << (int)propertyFlags << (bool)false;
    return;
}

//----------
void PlanQTests::verify_set_plan()
{
    setup();

    QFETCH(int, id);
    QFETCH(QVector<Waypt>, wayptList);
    QFETCH(float, planLength);
    QFETCH(float, crossTrack);
    QFETCH(int, propertyFlagsVal);
    QFETCH(bool, setOk);

    //set plan
    Plan plan;
    QString resString;
    QVERIFY(plan.getFieldFlags().testFlag(Plan::Field::NONE));
    bool res = plan.setPlan(wayptList, id, &resString);
    qInfo() << "[PlanQTests::verify_set_plan()]" << resString;
    QCOMPARE(res, setOk);

    //set crosstrack
    QVERIFY(!plan.getFieldFlags().testFlag(Plan::Field::CROSS_TRACK));
    plan.setCrossTrack(crossTrack);
    QVERIFY(plan.getFieldFlags().testFlag(Plan::Field::CROSS_TRACK));
    QCOMPARE(plan.crossTrack(), crossTrack);

    //set property flags
    Plan::PropertyFlags propertyFlags(propertyFlagsVal);
    QVERIFY(!plan.getFieldFlags().testFlag(Plan::Field::PROPERTY_FLAGS));
    plan.setPropertyFlags(propertyFlags);
    QVERIFY(plan.getFieldFlags().testFlag(Plan::Field::PROPERTY_FLAGS));
    QCOMPARE(plan.propertyFlags(), propertyFlags);

    if(res){
        //verify waypts are set correctly
        QVector<Waypt> wayptList_res = plan.wayptList();
        for(int i = 0; i < wayptList.size() && i < wayptList_res.size(); ++i){
            verifySame(wayptList.at(i), wayptList_res.at(i));
        }

        //verify field flags
        QVERIFY(plan.getFieldFlags().testFlag(Plan::Field::ID));
        QVERIFY(plan.getFieldFlags().testFlag(Plan::Field::SEGMENT_LIST));
        QVERIFY(plan.getFieldFlags().testFlag(Plan::Field::LENGTH));

        //verify other params
        QCOMPARE(plan.id(), id);
        QVERIFY(qFuzzyCompare(plan.length(), planLength));
        QCOMPARE(plan.nWaypt(), wayptList.size());
        QCOMPARE(plan.nSegment(), wayptList.size() - 1);
    }
    else{
        //verify field flags
        QVERIFY(!plan.getFieldFlags().testFlag(Plan::Field::ID));
        QVERIFY(!plan.getFieldFlags().testFlag(Plan::Field::SEGMENT_LIST));
        QVERIFY(!plan.getFieldFlags().testFlag(Plan::Field::LENGTH));
    }
}
