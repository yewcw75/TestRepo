/**
 * @file VectorF.h
 * @brief Wrapper for the underlying boost::numeric::ublas::vector<double> structure used to store coordinates or vectors used in this library.
 *
 * The VectorF class provides a convenient wrapper for the underlying boost::numeric::ublas::vector<double> structure. It allows for easy construction, resizing,
 * element access, and manipulation of vectors.
 *
 * With the underlying structure wrapped, it is hoped that in the event that a different underlying structure were to be used in the future, changes can be made more easily.
 * Furthermore, additional functionalities to make things more convenient can be made, e.g., constructing with std::initializer for a ublas::vector.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-06-30
 */

#ifndef RRTPLANNER_LIB_VECTOR_F_H
#define RRTPLANNER_LIB_VECTOR_F_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <QSharedDataPointer>
#include <QDebug>
#include <initializer_list>
#include <boost/numeric/ublas/vector.hpp>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VectorFPrivate;

/**
 * @class VectorF
 * @brief Wrapper for the underlying boost::numeric::ublas::vector<double> structure.
 *
 * The VectorF class provides a convenient wrapper for the underlying boost::numeric::ublas::vector<double> structure. It allows for easy construction, resizing,
 * element access, and manipulation of vectors.
 */
class RRTPLANNER_LIB_EXPORT VectorF
{
public:
    /**
     * @brief Default constructor. Constructs a VectorF object with a zero-size vector.
     */
    VectorF();

    /**
     * @brief Constructs a VectorF object from a std::initializer_list of double values.
     * @param list The list of double values used to initialize the vector.
     */
    explicit VectorF(const std::initializer_list<double>& list);

    /**
     * @brief Constructs a VectorF object from a boost::numeric::ublas::vector<double>.
     * @param data The boost::numeric::ublas::vector<double> used to initialize the vector.
     */
    explicit VectorF(const boost::numeric::ublas::vector<double>& data);

    /**
     * @brief Destructor.
     */
    ~VectorF();

    /**
     * @brief Copy constructor.
     */
    VectorF(const VectorF& other);

    /**
     * @brief Assignment operator.
     */
    VectorF& operator=(const VectorF& other);

    /**
     * @brief Resizes the vector to the specified size.
     * @param size The new size of the vector.
     * @param to_preserve_data Flag indicating whether to preserve the existing data when resizing.
     */
    void resize(int size, bool to_preserve_data = true);

    /**
     * @brief Returns the size of the vector.
     * @return The size of the vector.
     */
    int size() const;

    /**
     * @brief Returns value of the element at the specified index.
     * @param idx The index of the element.
     * @return A constant reference to the element at the specified index.
     */
    double at(int idx) const;

    /**
     * @brief Returns a value of the element at the specified index using the array subscript operator.
     * @param idx The index of the element.
     * @return A constant reference to the element at the specified index.
     */
    double operator[](int idx) const;

    /**
     * @brief Returns a reference to the element at the specified index using the array subscript operator.
     * @param idx The index of the element.
     * @return A reference to the element at the specified index.
     */
    double& operator[](int idx);

    /**
     * @brief Sets the data of the vector using a boost::numeric::ublas::vector<double>.
     * @param data The boost::numeric::ublas::vector<double> used to set the vector data.
     */
    void set(const boost::numeric::ublas::vector<double>& data);

    /**
     * @brief Returns a constant reference to the underlying boost::numeric::ublas::vector<double> data.
     * @return A constant reference to the underlying boost::numeric::ublas::vector<double> data.
     */
    const boost::numeric::ublas::vector<double>& data_const_ref() const;

    /**
     * @brief Returns a reference to the underlying boost::numeric::ublas::vector<double> data.
     * @return A reference to the underlying boost::numeric::ublas::vector<double> data.
     */
    boost::numeric::ublas::vector<double>& data();

    /**
     * @brief Overloads the << operator to output the VectorF object to the QDebug.
     * @param debug The QDebug object.
     * @param data The VectorF object to be output.
     * @return The modified QDebug object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::VectorF& data);

private:
    QSharedDataPointer<VectorFPrivate> d_ptr; /**< The QSharedDataPointer to the VectorF's private implementation. */
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
