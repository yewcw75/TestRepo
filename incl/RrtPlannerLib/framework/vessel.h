#ifndef RRTPLANNER_LIB_FRAMEWORK_VESSEL_H
#define RRTPLANNER_LIB_FRAMEWORK_VESSEL_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VesShape.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QSharedDataPointer>
#include <QSharedPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE
class VesselPrivate;
class RRTPLANNER_LIB_EXPORT Vessel
{
public:
    Vessel();
    Vessel(const VectorF &posNE, double lon0_deg, const VectorF &velNE, double hdg_deg);
    Vessel(const Vessel &);
    Vessel &operator=(const Vessel &);
    virtual ~Vessel();

    void setPosNE(const VectorF &posNE, double lon0_deg);
    const VectorF &posNE() const;
    void setPosNE(const VectorF &posNE);
    double lon0_deg() const;
    void setLon0_deg(double lon0_deg);
    const VectorF &velNE() const;
    void setVelNE(const VectorF &velNE);
    double hdg_deg() const;
    void setHdg_deg(double hdg_deg);

    void setVesShape(const QSharedPointer<VesShape>& p_vesShape);
    const QSharedPointer<VesShape>& vesShape() const; //will be nullptr if vessel shape had not been set.

    const QDateTime &timeStamp() const; //use QDateTime::isValid() to check if timestamp had been set
    void setTimeStamp(const QDateTime &newTimeStamp);
    void setTimeStamp(); //stamp current date time

private:
    QSharedDataPointer<VesselPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
