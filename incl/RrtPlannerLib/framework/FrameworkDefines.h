#ifndef RRTPLANNER_LIB_VECTOR_H
#define RRTPLANNER_LIB_VECTOR_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
//#include <boost/numeric/ublas/vector.hpp>
#include <boost/geometry.hpp>

#define DIM_COORD 2 //dimension of coordinates
#define IDX_NORTHING 0 //idx of northing element in Coord_NE, Vector_NE
#define IDX_EASTING 1  //idx of easting element in Coord_NE, Vector_NE
#define IDX_LAT 0 //idx of lat element in Coord_LatLon
#define IDX_LON 1 //idx of lon element in Coord_LatLon

#define TOL_SMALL 1e-6
#define EPS_DX 1e-3

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//using Coord_NE = boost::numeric::ublas::vector<float>; //[m][m] Coord in northing/easting
//using Vector_NE = boost::numeric::ublas::vector<float>; //[m][m] Vector in northing/easting
//using Coord_LatLon = boost::numeric::ublas::vector<double>; //[deg][deg] Coord in lat/lon

namespace bg = boost::geometry;
using Coord_NE = bg::model::point<float, DIM_COORD, bg::cs::cartesian>; //[m][m] Coord in northing/easting
using Vector_NE = bg::model::point<float, DIM_COORD, bg::cs::cartesian>; //[m][m] Vector in northing/easting
using Coord_LatLon = bg::model::point<double, DIM_COORD, bg::cs::geographic<bg::degree>>; //[deg][deg] Coord in lat/lon

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
