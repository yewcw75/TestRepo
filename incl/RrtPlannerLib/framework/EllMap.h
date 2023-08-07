/**
 * @file EllMap.h
 * This file contains the EllMap class.
 *
 * The EllMap class represents a map that consists of a nominal plan and offset plans on both sides about the nominal plan.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-06-30
 */

#ifndef RRTPLANNER_LIB_ELLMAP_H
#define RRTPLANNER_LIB_ELLMAP_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/RootData.h>
#include <QObject>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class EllMapPrivate;
/**
 * @class EllMap
 * @brief The EllMap class represents a map consisting of a nominal plan and offset plans on both sides about the nominal plan.
 *
 */
class RRTPLANNER_LIB_EXPORT EllMap
{
public:
    /**
     * @brief Default constructor for the EllMap class.
     */
    explicit EllMap();

    /**
     * @brief Deleted copy constructor.
     * @param rhs The EllMap object to copy.
     */
    EllMap(const EllMap& rhs);


    /**
     * @brief Destructor for the EllMap class.
     */
    virtual ~EllMap();

    /**
     * @brief Assignment operator
     * @param rhs The EllMap object to copy.
     */
    EllMap& operator=(const EllMap& rhs);

    /**
     * @brief Sets the nominal plan for the EllMap.
     * @param plan The nominal plan to set.
     *
     * Note: The plan ID of the stored nominal plan will be replaced with the index in EllMap's plan list after buildEllMap() is called.
     */
    void setNominalPlan(const Plan& plan);

    /**
     * @brief Builds an EllMap based on the currently set nominal plan.
     * @param crossTrackHorizon The maximum cross-track distance to generate offset plans [m].
     * @param results_desc Optional pointer to return the description of the result.
     * @return True if the map is successfully built, false otherwise.
     *
     * This function clears the current EllMap and builds a new EllMap based on the currently set nominal plan.
     */
    bool buildEllMap(double crossTrackHorizon, QString* results_desc = nullptr);

    /**
     * @brief Gets the number of plans in the EllMap.
     * @return The number of plans.
     */
    int nPlan() const;

    /**
     * @brief Gets the nominal plan of the EllMap.
     * @return A shared pointer to the nominal plan.
     */
    QSharedPointer<const Plan> nominalPlan() const;

    /**
     * @brief Gets the plan at the specified index in the EllMap.
     * @param idx The index of the plan to get.
     * @return A shared pointer to the plan at the specified index.
     */
    QSharedPointer<const Plan> at(int idx) const;

    /**
     * @brief Locate the sector in EllMap given a position.
     * @param[in] posNE Position in Northing-Easting [m] to query.
     * @param[in] planIdx_0 Initial plan idx to start the search.
     * @param[in] segIdx_0 Initial segment idx to start the search.
     * @param[out] planIdx plan idx associated with the found sector.
     * @param[out] segIdx segment idx associated with the found sector.
     * @return bool True if sector is found. False if given position is out of the Ellmap boundaries.
     */
    bool locateSector(const VectorF& posNE,
                      int planIdx_0, int segIdx_0,
                      int& planIdx, int& segIdx
                      ) const;

    bool getRootData(const VectorF& posNE, RootData& rootData) const;

    /**
     * @brief Overloads the << operator to output the EllMap object to the debug stream.
     * @param debug The debug stream.
     * @param data The EllMap object to output.
     * @return The debug stream with the EllMap object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::EllMap& data);

private:
    QSharedDataPointer<EllMapPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif // RRTPLANNER_LIB_ELLMAP_H
