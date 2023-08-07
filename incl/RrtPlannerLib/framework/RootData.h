/**
 * @file RootData.h
 * @brief RootData class.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-06-30
 */
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
    /**
     * @brief Default constructor.
     */
    RootData();

    /**
     * @brief Constructor with a specific position.
     * @param posNE The position vector.
     */
    RootData(const VectorF &posNE);

    /**
     * @brief Copy constructor.
     * @param other The RootData object to copy from.
     */
    RootData(const RootData& other);

    /**
     * @brief Copy assignment operator.
     * @param other The RootData object to assign from.
     * @return Reference to the assigned RootData object.
     */
    RootData &operator=(const RootData& other);

    /**
     * @brief Destructor.
     */
    virtual ~RootData();

    /**
     * @brief Get the set USV position.
     * @return The USV position vector.
     */
    const VectorF &posNE() const;

    /**
     * @brief Set the USV position.
     * @param posNE The USV position vector.
     */
    void setPosNE(const VectorF &posNE);

    /**
     * @brief Get the ell_list.
     * @return A reference to the ell_list.
     */
    QVector<double>& ell_list();

    /**
     * @brief Get a constant reference to the ell_list.
     * @return A constant reference to the ell_list.
     */
    const QVector<double>& ell_list_const_ref() const;

    /**
     * @brief Get the cross-track distance with respect to the nominal plan.
     * @return The cross-track distance in meters.
     */
    double dx() const;

    /**
     * @brief Set the cross-track distance with respect to the nominal plan.
     * @param dx The cross-track distance in meters.
     */
    void setDx(double dx);

    /**
     * @brief Get the length from start to current USV position.
     * @return The length in meters.
     */
    double ell() const;

    /**
     * @brief Set the length from start to current USV position.
     * @param ell The length in meters.
     */
    void setEll(double ell);

    /**
     * @brief Get the length of the segment that the USV position is on.
     * @return The segment length in meters.
     */
    double L() const;

    /**
     * @brief Set the length of the segment that the USV position is on.
     * @param l The segment length in meters.
     */
    void setL(double l);

    /**
     * @brief Get the fraction dl/L, where dl is the distance from the start of the current segment to the USV position.
     * @return The fraction value.
     */
    double f_ell() const;

    /**
     * @brief Set the fraction dl/L, where dl is the distance from the start of the current segment to the USV position.
     * @param f_ell The fraction value.
     */
    void setF_ell(double f_ell);

    /**
     * @brief Get the plan index associated with the polygon sector that the root is inside.
     * @return The plan index.
     */
    int planIdx() const;

    /**
     * @brief Set the plan index associated with the polygon sector that the root is inside.
     * @param planIdx The plan index.
     */
    void setPlanIdx(int planIdx);

    /**
     * @brief Get the segment index associated with the polygon sector that the root is inside.
     * @return The segment index.
     */
    int segIdx() const;

    /**
     * @brief Set the segment index associated with the polygon sector that the root is inside.
     * @param segIdx The segment index.
     */
    void setSegIdx(int segIdx);

    /**
     * @brief Check if the USV position is within the span of all the plans in EllMap.
     * @return `true` if the USV position is within the span, `false` otherwise.
     */
    bool isInPoly() const;

    /**
     * @brief Set whether the USV position is within the span of all the plans in EllMap.
     * @param isInPoly `true` if the USV position is within the span, `false` otherwise.
     */
    void setIsInPoly(bool isInPoly);

    /**
     * @brief Reset all parameters to their default values.
     */
    void reset();

    /**
     * @brief Overload of the << operator to output the RootData object to the debug stream.
     * @param debug The debug stream.
     * @param data The RootData object to output.
     * @return The debug stream with the RootData object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::RootData &data);

private:
    QSharedDataPointer<RootDataPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
