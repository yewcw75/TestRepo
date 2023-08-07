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

    //---Initial parameters for SMap--
    void setEllMap(const EllMap &ellMap,
                   double lh0, //arc-length horizon
                   double th0,  //time-horizon
                   double umin, //min speed
                   double umax  //max speed
                   );
    const EllMap &ellMap() const;
    double lh0() const;
    double th0() const;
    double umin() const;
    double umax() const;
    //----------------------------------

    //---Create/Reset SMap----------------
    bool reset(const VectorF& posNE);

    //----------------------------------

    //---getters to SMap----------------
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
    //----------------------------------

    /**
     * @brief Overloads the << operator to output the Plan object to the debug stream.
     * @param debug The debug stream.
     * @param data The SMap object to output.
     * @return The debug stream with the Plan object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::SMap &data);

protected:
    //for testing
    void setLh0(double lh0);
    void setTh0(double th0);
    void setUmin(double umin);
    void setUmax(double umax);

private:
    QSharedDataPointer<SMapPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif // SMAP_H
