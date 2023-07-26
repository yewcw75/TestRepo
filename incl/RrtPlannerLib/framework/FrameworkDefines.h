#ifndef RRTPLANNER_LIB_VECTOR_H
#define RRTPLANNER_LIB_VECTOR_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>

#define DIM_COORD 2 //dimension of coordinates
#define IDX_NORTHING 0 //idx of northing element in Coord_NE, Vector_NE
#define IDX_EASTING 1  //idx of easting element in Coord_NE, Vector_NE
#define IDX_LAT 0 //idx of lat element in Coord_LatLon
#define IDX_LON 1 //idx of lon element in Coord_LatLon

#define IDX_U 0 //idx of element in Vessel's coordinates in the aft direction
#define IDX_V 1 //idx of element in Vessel's coordinates in the stbd direction

#define TOL_SMALL 1e-6
#define EPS_DX 1e-3 //[m] cross track range to group edge events.

#endif
