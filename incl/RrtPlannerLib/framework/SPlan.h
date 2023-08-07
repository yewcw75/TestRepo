/**
 * @file SPlan.h
 * @brief
 * @authors Enric Xargay Mata, ycw
 * @date 20230804
 */

#ifndef RRTPLANNER_LIB_S_PLAN_H
#define RRTPLANNER_LIB_S_PLAN_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SPlanPrivate;
class RRTPLANNER_LIB_EXPORT SPlan
{
public:
    /**
     * @brief Default constructor.
     */
    SPlan();

    SPlan(double crossTrack, double Lh, double vol_cum, double area_cum);

    /**
     * @brief Destructor.
     */
    virtual ~SPlan();

    /**
     * @brief Copy constructor.
     */
    SPlan(const SPlan& other);

    /**
     * @brief Assignment operator.
     */
    SPlan& operator=(const SPlan& other);

    double getCrosstrack() const;
    void setCrosstrack(double crosstrack);
    double getLh() const;
    void setLh(double lh);
    double getVol_cum() const;
    void setVol_cum(double vol_cum);
    double getArea_cum() const;
    void setArea_cum(double area_cum);

    /**
     * @brief Overloads the << operator to output the Plan object to the debug stream.
     * @param debug The debug stream.
     * @param data The SMap object to output.
     * @return The debug stream with the Plan object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::SPlan &data);


private:
    QSharedDataPointer<SPlanPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif // RRTPLANNER_LIB_S_PLAN_H
