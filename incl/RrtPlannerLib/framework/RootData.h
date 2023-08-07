#ifndef RRTPLANNER_LIB_ROOTDATA_H
#define RRTPLANNER_LIB_ROOTDATA_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE
class RootDataPrivate;
class RRTPLANNER_LIB_EXPORT RootData
{
public:
    RootData();
    RootData(const VectorF &posNE);
    RootData(const RootData& other);
    RootData &operator=(const RootData& other);
    virtual ~RootData();

    const VectorF &posNE() const; //usv position.
    void setPosNE(const VectorF &posNE);

    //get ell_list
    QVector<double>& ell_list();
    const QVector<double>& ell_list_const_ref() const;

    //params of offset plan at usv position.
    double dx() const; //[m] cross-track dist. wrt nominal plan.
    void setDx(double dx);
    double ell() const; //[m] length from start to current usv position.
    void setEll(double ell);
    double L() const; //[m] length of segment that usv position is on.
    void setL(double l);
    double f_ell() const; //fraction dl/L, where dl is the distance from start of current segment to usv position.
    void setF_ell(double f_ell);

    //params for the polygon sector in which the root is inside.
    int planIdx() const; //plan idx associated with the polygon sector that root is inside.
    void setPlanIdx(int planIdx);
    int segIdx() const; //segment idx associated with the polygon sector that root is inside.
    void setSegIdx(int segIdx);
    bool isInPoly() const; //usv position is within the span of all the plans in EllMap.
    void setIsInPoly(bool isInPoly);

    void reset(); //resets all params

    /**
     * @brief Overloads the << operator to output the RootData object to the debug stream.
     * @param debug The debug stream.
     * @param data The RootData object to output.
     * @return The debug stream with the Plan object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::RootData &data);

private:
    QSharedDataPointer<RootDataPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
