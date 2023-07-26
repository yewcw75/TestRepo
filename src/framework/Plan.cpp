#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <QSharedData>
#include <QVector>
#include <QMetaEnum>
#include <QString>
#include <QtGlobal>
#include <QDebug>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class PlanPrivate: public QSharedData
{
public:
    PlanPrivate() = default;
    ~PlanPrivate() = default;
    PlanPrivate(const PlanPrivate& other) = default;

public:
    QVector<Segment> m_segmentList;
    Plan::PropertyFlags m_propertyFlags{};
    double m_crossTrack{}; //[m]
    int m_id{};
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
    mp_pimpl->m_crossTrack = 0.0;
    mp_pimpl->m_id = 0.0;
}

//----------
bool Plan::setPlan(const QVector<Waypt>& wayptList, //waypt list to set plan
             const QVector<int>& segIdList, //segment id to assign. Vector size expected to be (wayptList.size() - 1)
             QString* resultsDesc //optional QString ptr to return description of results
             )
{
    bool res_ok = setPlan(wayptList, segIdList, resultsDesc, true);
    return(res_ok);
}

//----------
bool Plan::setPlan(const QVector<Waypt>& wayptList, //waypt list to set plan
             const QVector<int>& segIdList, //segment id to assign. Vector size expected to be (wayptList.size() - 1)
             int id, //plan id.
             QString* resultsDesc //optional QString ptr to return description of results
             )
{
    bool res_ok = setPlan(wayptList, segIdList, resultsDesc, true);
    if(res_ok){
        setId(id);
    }
    return(res_ok);

}

//----------
bool  Plan::setPlan(const QVector<Waypt>& wayptList, //waypt list to set plan
             int id, //plan id.
             QString* resultsDesc//optional QString ptr to return description of results
             )
{
    return(setPlan(wayptList,  QVector<int>(), id, resultsDesc));
}

//----------
void Plan::setId(int id)
{
    mp_pimpl->m_id = id;
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

double Plan::length() const
{
    double length = 0.0;
    if(mp_pimpl->m_segmentList.size() > 0){
        length = mp_pimpl->m_segmentList.last().lengthCumulative();
    }
    return(length);
}

//----------
void Plan::setCrossTrack(double crossTrack)
{
    mp_pimpl->m_crossTrack = crossTrack;
}

//----------
double Plan::crossTrack() const
{
    return(mp_pimpl->m_crossTrack);
}

//----------
void Plan::setProperty(const Property& property, bool on)
{
    mp_pimpl->m_propertyFlags.setFlag(property, on);
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
}

//----------
const Plan::PropertyFlags& Plan::propertyFlags() const
{
    return(mp_pimpl->m_propertyFlags);
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::Plan &data)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "\n\n********************" << "\n*    Plan id: " << data.id() << "    *\n********************" << \
                       "\n  Crosstrack = " << data.crossTrack() << "m" << \
                       "\n  Length = " << data.length() << "m" << \
                       "\n  IsNominal = " << data.testProperty(Plan::Property::IS_NOMINAL) << \
                       "\n  IsLimit = " << data.testProperty(Plan::Property::IS_LIMIT);
    for(const Segment& segment: data.segmentList()){
        debug.nospace() << "\n  " << segment;
    }
    return debug;
}

//----------
bool Plan::setPlan(const QVector<Waypt>& wayptList, //waypt list to set plan
             const QVector<int>& segIdList, //segment id to assign. Vector size expected to be (wayptList.size() - 1)
             QString* resultsDesc, //optional QString ptr to return description of results
             bool toVerifyPlan)
{
    QVector<int> segIdList2Use(segIdList); //make a non-const copy for local use

    //if segIdList is not provided, we auto generate here
    if(segIdList2Use.size() == 0){
        for(int i = 0; i < wayptList.size() -1 ; ++i){
            segIdList2Use.append(i);
        }
    }

    //Check nWaypts and nSegIdList are correct
    int nWaypt = wayptList.size();
    int nSegIdList = segIdList2Use.size();

    bool res_ok = nWaypt-1 == nSegIdList;
    if(resultsDesc){ //write results description if given pointer is not null
        *resultsDesc = QString("[Plan::setPlan] Verify nWaypt-1 == nSegIdList: %1.").arg(res_ok);
    }

    //Check waypt plan is feasible
    if(res_ok && toVerifyPlan){
        PlanHelper::VerifyPlanResult res = PlanHelper::verifyPlanInput(wayptList);
        res_ok = (res == PlanHelper::VerifyPlanResult::VERIFY_PLAN_OK);
        if(resultsDesc){ //write results description if given pointer is not null. Overwrite previous.
            *resultsDesc = QString(" Verify waypoints: ") + QMetaEnum::fromType<PlanHelper::VerifyPlanResult>().key(static_cast<int>(res));
        }
    }

    //start to build plan if verify plan is ok
    if(res_ok){
        mp_pimpl->m_segmentList.clear();

        //form segment and append to segment list
        double lengthCumulative = 0;
        Waypt wayptPrev = wayptList.at(0);
        for(int idx = 1; idx < nWaypt; ++idx){
            Waypt wayptNext = wayptList.at(idx);
            Segment segment(wayptPrev, wayptNext, segIdList2Use.at(idx - 1));
            segment.setSegmentAttributes(); //set length, tVec, nVec
            lengthCumulative += segment.length();
            segment.setLengthCumulative(lengthCumulative);

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

            //for next iteration
            wayptPrev = wayptNext;
        }
    }

    return(res_ok);
}

//----------
void Plan::appendSegment(const Segment& segment)
{
    double lengthCumulative = this->length() + segment.length(); //expected cumulative length
    mp_pimpl->m_segmentList.append(segment);
    mp_pimpl->m_segmentList.last().setLengthCumulative(lengthCumulative);
}

//----------
void Plan::setSegmentList(const QVector<Segment>& segmentList)
{
    mp_pimpl->m_segmentList = segmentList;
}
//----------

RRTPLANNER_FRAMEWORK_END_NAMESPACE
