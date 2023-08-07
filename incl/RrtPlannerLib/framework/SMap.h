/**
 * @file SMap.h
 * @brief This file contains the declaration of the SMap class.
 * @authors Enric Xargay Mata, ycw
 * @date 2023-08-04
 */

#ifndef RRTPLANNER_LIB_S_MAP_H
#define RRTPLANNER_LIB_S_MAP_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/SPlan.h>
#include <RrtPlannerLib/framework/EllMap.h>
#include <QSharedDataPointer>
#include <QSharedPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

/**
 * @class SMap
 */
class SMapPrivate;
class RRTPLANNER_LIB_EXPORT SMap
{
    friend class SMapHelper;
public:
    /**
     * @brief Default constructor.
     */
    SMap();

    /**
     * @brief Copy constructor.
     */
    SMap(const SMap& other);

    /**
     * @brief Assignment operator.
     */
    SMap& operator=(const SMap& other);

    /**
     * @brief Destructor.
     */
    virtual ~SMap();

    //---Initial parameters for SMap--
    /**
     * @brief Set the initial parameters for the SMap.
     * @param ellMap The EllMap object.
     * @param lh0 The desired arc-length horizon.
     * @param th0 The desired time-horizon.
     * @param umin The minimum speed.
     * @param umax The maximum speed.
     */
    void setEllMap(const EllMap &ellMap,
                   double lh0,
                   double th0,
                   double umin,
                   double umax
                   );

    /**
     * @brief Get the underlying EllMap associated with the SMap.
     * @return The EllMap object.
     */
    const EllMap &ellMap() const;

    /**
     * @brief Get the arc-length horizon.
     * @return The arc-length horizon.
     */
    double lh0() const;

    /**
     * @brief Get the time-horizon.
     * @return The time-horizon.
     */
    double th0() const;

    /**
     * @brief Get the minimum speed.
     * @return The minimum speed.
     */
    double umin() const;

    /**
     * @brief Get the maximum speed.
     * @return The maximum speed.
     */
    double umax() const;
    //----------------------------------

    //---Create/Reset SMap----------------
    /**
     * @brief Reset the SMap with a given position.
     * @param posNE The position vector.
     * @return `true` if successful, `false` otherwise.
     */
    [[nodiscard]] bool reset(const VectorF& posNE);

    //----------------------------------

    //---getters to SMap----------------
    /**
     * @brief Get the number of plans in the SMap.
     * @return The number of plans.
     */
    int size() const;

    /**
     * @brief Get a constant reference to the SPlan at a specific index.
     * @param idx The index of the SPlan.
     * @return The SPlan object.
     */
    const SPlan& at(int idx) const;

    /**
     * @brief Get a constant reference to the last SPlan in the SMap.
     * @return The last SPlan object.
     */
    const SPlan& last() const;


    /**
     * @brief Get a constant reference to the first SPlan in the SMap.
     * @return The first SPlan object.
     */
    const SPlan& first() const;

    /**
     * @brief Get a constant reference to the list of SPlan objects in the SMap.
     * @return The list of SPlan objects.
     */
    const QList<SPlan>& SPlanList_const_ref() const;

    /**
     * @brief Get the index of the nominal plan.
     * @return The index of the nominal plan.
     */
    int idxNominal() const;

    //----------------------------------

    /**
     * @brief Overload of the << operator to output the SMap object to the debug stream.
     * @param debug The debug stream.
     * @param data The SMap object to output.
     * @return The debug stream with the SMap object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::SMap &data);

protected:
    //
    /**
     * @brief Append a plan to the SMap.
     * @param sPlan The SPlan object to append.
     */
    void append(const SPlan& sPlan);

    /**
     * @brief Get a reference to the SPlan at a specific index.
     * @param idx The index of the SPlan.
     * @return The SPlan object.
     */
    SPlan& operator[](int idx);

    /**
     * @brief Get a reference to the last SPlan in the SMap.
     * @return The last SPlan object.
     */
    SPlan& last();

    /**
     * @brief Get a reference to the first SPlan in the SMap.
     * @return The first SPlan object.
     */
    SPlan& first();

    /**
     * @brief Get a reference to the list of SPlan objects in the SMap.
     * @return The list of SPlan objects.
     */
    QList<SPlan>& SPlanList();

    /**
     * @brief Set the list of SPlan objects in the SMap.
     * @param SPlanList The list of SPlan objects.
     */
    void setSPlanList(const QList<SPlan>& SPlanList);

    /**
     * @brief Set the index of the nominal plan.
     * @param idxNominal The index of the nominal plan.
     */
    void setIdxNominal(int idxNominal);


    //for testing
    /**
     * @brief Set the arc-length horizon.
     * @param lh0 The arc-length horizon.
     */
    void setLh0(double lh0);

    /**
     * @brief Set the time-horizon.
     * @param th0 The time-horizon.
     */
    void setTh0(double th0);

    /**
     * @brief Set the minimum speed.
     * @param umin The minimum speed.
     */
    void setUmin(double umin);

    /**
     * @brief Set the maximum speed.
     * @param umax The maximum speed.
     */
    void setUmax(double umax);

private:
    QSharedDataPointer<SMapPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif // SMAP_H
