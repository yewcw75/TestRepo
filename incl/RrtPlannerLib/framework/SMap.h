/**
 * @file SMap.h
 * @brief
 * @authors Enric Xargay Mata, ycw
 * @date 20230804
 */

#ifndef RRTPLANNER_LIB_S_MAP_H
#define RRTPLANNER_LIB_S_MAP_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/SPlan.h>
#include <RrtPlannerLib/framework/EllMap.h>
#include <RrtPlannerLib/framework/RootData.h>
#include <QSharedDataPointer>
#include <QSharedPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SMapPrivate;
class RRTPLANNER_LIB_EXPORT SMap
{
public:
    SMap();
    SMap(const SMap& other);
    SMap& operator=(const SMap& other);
    virtual ~SMap();

//    void create(const EllMap& ellMap,       //ellmap input
//                const RootData& root_data,  //data relevant to current usv's position
//                double lh0,                 //[m] desired arclength horizon
//                double th0,                 //[s] desired time horizon
//                double umin,                //[m/s] min speed
//                double umax                 //[m/s] max speed
//                );

    int size() const;
    void append(const SPlan& sPlan);
    const SPlan& at(int idx) const;
    SPlan& operator[](int idx);
    const SPlan& last() const;
    SPlan& last();
    const SPlan& first() const;
    SPlan& first();

    const QList<SPlan>& SPlanList_const_ref() const;
    QList<SPlan>& SPlanList();
    void setSPlanList(const QList<SPlan>& SPlanList);

    int idxNominal() const;
    void setIdxNominal(int idxNominal);

    /**
     * @brief Overloads the << operator to output the Plan object to the debug stream.
     * @param debug The debug stream.
     * @param data The SMap object to output.
     * @return The debug stream with the Plan object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::SMap &data);

private:
    QSharedDataPointer<SMapPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif // SMAP_H
