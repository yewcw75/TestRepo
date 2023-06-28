#ifndef RRTPLANNER_LIB_PLAN_H
#define RRTPLANNER_LIB_PLAN_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/Segment.h>
#include <QSharedDataPointer>
#include <QObject>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class PlanPrivate;
class RRTPLANNER_LIB_EXPORT Plan
{
    Q_GADGET
public:
    enum class Field{
        NONE            = 0,
        ID              = 1 << 1,
        SEGMENT_LIST    = 1 << 2,
        LENGTH          = 1 << 3,
        CROSS_TRACK     = 1 << 4,
        PROPERTY_FLAGS  = 1 << 5
    };
    Q_FLAG(Field) //Q_ENUM is also called in Q_FLAG()
    Q_DECLARE_FLAGS(FieldFlags, Field) //FieldFlags to indicate fields that had been set

    enum class Property{
        NONE            = 0,
        IS_NOMINAL      = 1 << 0,
        IS_LIMIT        = 1 << 1,
    };
    Q_FLAG(Property) //Q_ENUM is also called in Q_FLAG()
    Q_DECLARE_FLAGS(PropertyFlags, Property)

public:
    Plan();
    virtual ~Plan();
    Plan(const Plan& other);
    Plan& operator=(const Plan& other);

    void clearPlan(); //clear plan and reset all parameters and property flags
    bool setPlan(const QVector<Waypt>& wayptList, //waypt list to set plan
                 int id, //plan id
                 QString* resultsDesc = nullptr //optional QString ptr to return description of results
                 );
    int id() const; //plan id
    int nSegment() const; //no. of segments
    int nWaypt() const; //no. of waypts ( = nSegment + 1)
    QVector<Waypt> wayptList() const; //get the waypt list of plan
    const QVector<Segment>& segmentList() const; //get the segment list of plan

    float length() const; //[m] total plan length

    void setCrossTrack(float crossTrack); //[m] cross-track dist to nominal plan
    float crossTrack() const; //[m] //[m] cross-track dist to nominal plan

    void setProperty(const Property& property, bool on = true);
    bool testProperty(const Property& property) const;

    void setPropertyFlags(const PropertyFlags& flags);
    const PropertyFlags& propertyFlags() const;

    //presence flags of fields
    const FieldFlags& getFieldFlags() const;

private:
    void appendSegment(const Segment& segment);

private:
    QSharedDataPointer<PlanPrivate> mp_pimpl;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Plan::FieldFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Plan::PropertyFlags)

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
