#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <QSharedData>
#include <QVector>
#include <QMetaEnum>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class PlanPrivate: public QSharedData
{
public:
    PlanPrivate()
        :QSharedData()
    {}
    ~PlanPrivate()
    {}
    PlanPrivate(const PlanPrivate& other)
        :QSharedData(other),
          m_segmentList(other.m_segmentList),
          m_propertyFlags(other.m_propertyFlags),
          m_fieldFlags(other.m_fieldFlags),
          m_length(other.m_length),
          m_crossTrack(other.m_crossTrack),
          m_id(other.m_id)
    {}

public:
    QVector<Segment> m_segmentList;
    Plan::PropertyFlags m_propertyFlags{};
    Plan::FieldFlags m_fieldFlags{};
    float m_length{}; //[m]
    float m_crossTrack{}; //[m]
    int m_id;
};

//##########################
//----------
Plan::Plan()
    :mp_pimpl(new PlanPrivate())
{

}

//----------
Plan::~Plan()
{

}

//----------
Plan::Plan(const Plan& other)
    :mp_pimpl(other.mp_pimpl)
{

}

//----------
Plan& Plan::operator=(const Plan& other)
{
    if(this != &other){
        mp_pimpl = other.mp_pimpl;
    }
    return(*this);
}

//----------
void Plan::clearPlan()
{
    mp_pimpl->m_segmentList.clear();
    mp_pimpl->m_propertyFlags = Property::NONE;
    mp_pimpl->m_fieldFlags = Field::NONE;
    mp_pimpl->m_length = 0.0;
    mp_pimpl->m_crossTrack = 0.0;
    mp_pimpl->m_id = 0.0;
    return;
}

//----------
bool Plan::setPlan(const QVector<Waypt>& wayptList, //waypt list to set plan
                   int id, //plan id
                   QString* resultsDesc //optional QString ptr to return description of results
                   )
{

    int nWaypt = wayptList.size();
    PlanHelper::VerifyPlanResult res = PlanHelper::verifyPlanInput(wayptList);
    bool res_ok = (res == PlanHelper::VerifyPlanResult::VERIFY_PLAN_OK);

    if(resultsDesc){ //write results description if given pointer is not null
        *resultsDesc = QMetaEnum::fromType<PlanHelper::VerifyPlanResult>().key(static_cast<int>(res));
    }

    //set plan is verify plan is ok
    if(res_ok){
        mp_pimpl->m_id = id;
        mp_pimpl->m_fieldFlags.setFlag(Field::ID);

        //form segment and append to segment list
        Waypt wayptPrev = wayptList.at(0);
        for(int idx = 1; idx < nWaypt; ++idx){
            Waypt wayptNext = wayptList.at(idx);
            Segment segment(wayptPrev, wayptNext, idx - 1);

            //bisector
            if(idx == 1){ //now is first segment
                segment.setbVecPrev(segment.nVec());
            }
            else{
                Segment& segmentPrev = mp_pimpl->m_segmentList.last();
                segment.setbVecPrev(segmentPrev);
                segmentPrev.setbVecNext(segment.bVecPrev());
            }

            if(idx == nWaypt - 1){ //now is last segment
                segment.setbVecNext(segment.nVec());
            }

            //add segment
            mp_pimpl->m_segmentList.append(segment);
            mp_pimpl->m_length += segment.length(); //accumulate length for plan

            //for next iteration
            wayptPrev = wayptNext;
        }
        mp_pimpl->m_fieldFlags.setFlag(Field::SEGMENT_LIST);
        mp_pimpl->m_fieldFlags.setFlag(Field::LENGTH);
    }

    return(res_ok);
}

//----------
int Plan::id() const
{
    return(mp_pimpl->m_id);
}

//----------
int Plan::nSegment() const
{
    return(mp_pimpl->m_segmentList.size());
}

//----------
int Plan::nWaypt() const
{
    return(mp_pimpl->m_segmentList.size() + 1);
}

//----------
/**
 * Developer notes:
 * - Note that a local QVector<Waypt> res is used to store the waypts of the plan.
 * - Do not change to return by reference (i.e. return const QVector<Waypt>&) as res will be invalid after this function returns.
 */
QVector<Waypt> Plan::wayptList() const
{
    QVector<Waypt> res;
    QVector<Segment>::const_iterator it;
    for(it = mp_pimpl->m_segmentList.begin();
        it != mp_pimpl->m_segmentList.end();
        ++it){
        if(it == mp_pimpl->m_segmentList.begin()){
            res.append(it->wayptPrev());
        }
        res.append(it->wayptNext());
    }
    return(res);
}

//----------
const QVector<Segment>& Plan::segmentList() const
{
    return(mp_pimpl->m_segmentList);
}

//----------

float Plan::length() const
{
    return(mp_pimpl->m_length);
}

//----------
void Plan::setCrossTrack(float crossTrack)
{
    mp_pimpl->m_crossTrack = crossTrack;
    mp_pimpl->m_fieldFlags.setFlag(Field::CROSS_TRACK);
    return;
}

//----------
float Plan::crossTrack() const
{
    return(mp_pimpl->m_crossTrack);
}

//----------
void Plan::setProperty(const Property& property, bool on)
{
    mp_pimpl->m_propertyFlags.setFlag(property, on);
    mp_pimpl->m_fieldFlags.setFlag(Field::PROPERTY_FLAGS);
}

//----------
bool Plan::testProperty(const Property& property) const
{
    return(mp_pimpl->m_propertyFlags.testFlag(property));
}

//----------
void Plan::setPropertyFlags(const PropertyFlags& flags)
{
    mp_pimpl->m_propertyFlags = flags;
    mp_pimpl->m_fieldFlags.setFlag(Field::PROPERTY_FLAGS);
    return;
}

//----------
const Plan::PropertyFlags& Plan::propertyFlags() const
{
    return(mp_pimpl->m_propertyFlags);
}

//----------
const Plan::FieldFlags& Plan::getFieldFlags() const
{
    return(mp_pimpl->m_fieldFlags);
}

//----------
void Plan::appendSegment(const Segment& segment)
{
    mp_pimpl->m_segmentList.append(segment);
    mp_pimpl->m_length += segment.length(); //accumulate length for plan
    mp_pimpl->m_fieldFlags.setFlag(Field::SEGMENT_LIST);
    mp_pimpl->m_fieldFlags.setFlag(Field::LENGTH);
    return;
}

//----------

RRTPLANNER_FRAMEWORK_END_NAMESPACE
