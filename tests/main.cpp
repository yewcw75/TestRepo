#include "WayptQTests.h"
#include "SegmentQTests.h"
#include "PlanQTests.h"
#include "PlanHelperQTests.h"
#include "UblasHelperQTests.h"
#include "VectorFQTests.h"
#include "VectorFHelperQTests.h"
#include "EllMapQTests.h"
#include "SMapQTests.h"
#include "SMapHelperQTests.h"

#include "VesRectangleQTests.h"
#include "VesselQTests.h"
#include "RootDataQTests.h"

#include "PolygonQTests.h"
#include "SimplexQTests.h"
#include "GjkQTests.h"
#include <QtTest/QtTest>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    VectorFQTests       vectorFQTests;
    VectorFHelperQTests vectorFHelperQTests;
    WayptQTests         wayptQTests;
    SegmentQTests       segmentQTests;
    PlanQTests          planQTests;
    PlanHelperQTests    planHelperQTests;
    UblasHelperQTests   linearAlgebraHelperQTests;
    EllMapQTests        ellMapQTests;
    SMapQTests          sMapQTests;
    SMapHelperQTests    sMapHelperQTests;

    VesRectangleQTests  vesRectangleQTests;
    VesselQTests        vesselQTests;
    RootDataQTests      rootDataQTests;

    PolygonQTests       polygonQTests;
    SimplexQTests       simplexQTests;
    GjkQTests           gjkQTests;

    int status = \
            QTest::qExec(&vectorFQTests, argc, argv) + \
            QTest::qExec(&vectorFHelperQTests, argc, argv) + \
            QTest::qExec(&wayptQTests, argc, argv) + \
            QTest::qExec(&segmentQTests, argc, argv) + \
            QTest::qExec(&planQTests, argc, argv) + \
            QTest::qExec(&planHelperQTests, argc, argv) + \
            QTest::qExec(&linearAlgebraHelperQTests, argc, argv) + \
            QTest::qExec(&ellMapQTests, argc, argv) + \
            QTest::qExec(&sMapQTests, argc, argv) + \
            QTest::qExec(&sMapHelperQTests, argc, argv) + \

            QTest::qExec(&vesRectangleQTests, argc, argv) + \
            QTest::qExec(&vesselQTests, argc, argv) + \
            QTest::qExec(&rootDataQTests, argc, argv) + \

            QTest::qExec(&polygonQTests, argc, argv) + \
            QTest::qExec(&simplexQTests, argc, argv) + \
            QTest::qExec(&gjkQTests, argc, argv);

    return status;
}
