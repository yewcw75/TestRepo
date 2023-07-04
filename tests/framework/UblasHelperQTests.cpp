#include "UblasHelperQTests.h"
#include <RrtPlannerLib/framework/UblasHelper.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <QtTest/QtTest>
#include <QtGlobal>
#include <iostream>

using namespace rrtplanner::framework;
namespace bnu = boost::numeric::ublas;

//----------
UblasHelperQTests::UblasHelperQTests()
{

}

//----------
UblasHelperQTests::~UblasHelperQTests()
{

}

//----------
void UblasHelperQTests::verify_solve_data()
{
    using matrixF = bnu::matrix<double>;
    using vectorF = bnu::vector<double>;

    QTest::addColumn<bnu::matrix<double>>("M");
    QTest::addColumn<bnu::vector<double>>("y");
    QTest::addColumn<bnu::vector<double>>("x_expect");
    QTest::addColumn<bool>("res_expect");


    //dim 2
    matrixF M1(2, 2);
    M1(0, 0) = 1.0; M1(0, 1) = -3.0; M1(1, 0) = 5.0; M1(1, 1) = 2.0;
    vectorF y1(2);
    y1(0) = (double)4.0; y1(1) =  (double)-2.0;
    vectorF x1(2);
    x1(0) = (double)0.117647; x1(1) = (double)-1.294118;

    QTest::newRow("Test - dim 2") << M1 << y1 << x1 << true;

    //dim 3
    matrixF M2(3, 3);
    M2(0, 0) = 1.0; M2(0, 1) = -3.0; M2(0, 2) = 5.0;
    M2(1, 0) = 5.0; M2(1, 1) = 2.0;  M2(1, 2) = 4.0;
    M2(2, 0) = 1.0; M2(2, 1) = 2.0;  M2(2, 2) = 3.0;
    vectorF y2(3);
    y2(0) = (double)4.0; y2(1) =  (double)-2.0; y2(2) =  (double)3.0;
    vectorF x2(3);
    x2(0) = (double)-1.577465; x2(1) = (double)0.323944; x2(2) = (double)1.309859;
    QTest::newRow("Test - dim 3") << M2 << y2 << x2 << true;

    //dim 3 - singular
    matrixF M3(3, 3);
    M3(0, 0) = 1.0; M3(0, 1) = -3.0; M3(0, 2) = 5.0;
    M3(1, 0) = 5.0; M3(1, 1) = 2.0;  M3(1, 2) = 4.0;
    M3(2, 0) = 5.0; M3(2, 1) = 2.0;  M3(2, 2) = 4.0;
    vectorF y3(3);
    y3(0) = (double)4.0; y3(1) =  (double)-2.0; y3(2) =  (double)3.0; /*does not matter*/
    vectorF x3(3);
    x3(0) = (double)-1.577465; x3(1) = (double)0.323944; x3(2) = (double)1.309859; /*does not matter*/
    QTest::newRow("Test - dim 3 (singular)") << M3 << y3 << x3 << false;

}

//----------
void UblasHelperQTests::verify_solve()
{
    using matrixF = bnu::matrix<double>;
    using vectorF = bnu::vector<double>;

    QFETCH(matrixF, M);
    QFETCH(vectorF, y);
    QFETCH(vectorF, x_expect);
    QFETCH(bool, res_expect);

    vectorF x;
    bool res = UblasHelper::solve(M, y, 1e-6, x);

    QCOMPARE(res, res_expect);
    for(int i = 0; i < (int)x.size(); ++i){
        QVERIFY(UtilHelper::compare(x[i], x_expect[i]));
    }

    return;
}

//----------
/*
void UblasHelperQTests::verify_to_bnu_vector()
{
    double northing = -3.5, easting = 5.5;
    Bgm_Point vec{northing, easting};
    bnu::vector<double> bnu_vec = UblasHelper::to_bnu_vector(vec);
    QCOMPARE(bnu_vec[0], northing);
    QCOMPARE(bnu_vec[1], easting);
    return;
}
*/
//----------
/*
void UblasHelperQTests::verify_to_bgm_vector()
{
    double northing = -3.5, easting = 5.5;
    bnu::vector<double> bnu_vec(2);
    bnu_vec[0] = northing; bnu_vec[1] = easting;
    Bgm_Point point = UblasHelper::to_bgm_point(bnu_vec);
    QCOMPARE(point.get<0>(), northing);
    QCOMPARE(point.get<1>(), easting);
    return;
}
*/
//----------
void UblasHelperQTests::verify_concatenate_col_vectors()
{
    double a = 1.0, b = 2.0, c = 3.0, d = 4.0;
    bnu::vector<double> v1(2), v2(2);
    v1[0] = a, v1[1] = b;
    v2[0] = c, v2[1] = d;
    bnu::matrix<double> M = UblasHelper::concatenate_col_vectors(v1, v2);
    QCOMPARE(M.size1(), 2);
    QCOMPARE(M.size2(), 2);
    QCOMPARE(M(0,0), a);
    QCOMPARE(M(1,0), b);
    QCOMPARE(M(0,1), c);
    QCOMPARE(M(1,1), d);
    return;
}
