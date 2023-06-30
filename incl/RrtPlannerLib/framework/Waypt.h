/**
 * @file Waypt.h
 * Waypt class to store waypoints of a plan.
 *
 * @author: ycw
 * @date 20230630
 */
#ifndef RRTPLANNER_LIB_WAYPT_H
#define RRTPLANNER_LIB_WAYPT_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <QSharedDataPointer>
#include <QObject>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class WayptPrivate;
class RRTPLANNER_LIB_EXPORT Waypt
{
    Q_GADGET //needed for Q_FLAG macro
public:
    enum class Field{
        NONE      = 0,
        ID        = 1 << 0,
        NORTHING  = 1 << 1,
        EASTING   = 1 << 2,
        LON0      = 1 << 3,
    };
    Q_FLAG(Field) //Q_ENUM is also called in Q_FLAG()
    Q_DECLARE_FLAGS(FieldFlags, Field) //FieldFlags to keep track of fields that had been set

public:
    Waypt();
    Waypt(float northing_m, float easting_m, double lon0_deg = 0.0, int id = -1);
    Waypt(const VectorF& coord, double lon0_deg = 0.0, int id = -1);
    virtual ~Waypt();
    Waypt(const Waypt& other);
    Waypt& operator=(const Waypt& other);

    void set(float northing_m, float easting_m, double lon0_deg = 0.0, int id = -1);
    void set(const VectorF& coord, double lon0_deg = 0.0, int id = -1);

    void setNorthing(float northing_m); //[m] set northing
    float northing() const; //[m] northing
    void setEasting(float easting_m); //[m] set easting
    float easting() const; //[m] eastsing
    void setId(int id);
    int id() const;
    void setLon0(double lon0_deg); //[deg] reference longitude to convert to lat/lon
    double lon0_deg() const; //[deg] reference longitude to convert to lat/lon
    void setCoord(const VectorF& coord); //[m][m] set northing/easting
    const VectorF& coord_const_ref() const; //[m][m] northing/easting

    //presence flags of fields
    FieldFlags getFieldFlags() const;

private:
    QSharedDataPointer<WayptPrivate> mp_pimpl;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Waypt::FieldFlags)

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
