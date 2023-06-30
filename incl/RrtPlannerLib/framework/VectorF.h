/**
 * @VectorF.h
 * Wrapper for the underlying boost::numeric::ublas::vector<float> structure used to store coordinates or
 * vectors used in this library.
 *
 * With the underlying structure wrapped, it is hoped that in the event that a different underlying structure
 * were to be used in future, changes can be made more easily.
 * Furthermore, additional functionalities to make things more convenient can be made, e.g. constructing with std::initializer for
 * a ublas::vector.
 *
 *
 * @author ycw
 * @date 20230630
 */
#ifndef RRTPLANNER_LIB_VECTOR_F_H
#define RRTPLANNER_LIB_VECTOR_F_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <QSharedDataPointer>
#include <initializer_list>
#include <iostream>
#include <boost/numeric/ublas/vector.hpp>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VectorFPrivate;
class RRTPLANNER_LIB_EXPORT VectorF
{
public:
    VectorF(); //will construct with 0 size vector
    explicit VectorF(const std::initializer_list<float>& list); //construct with a list of float values
    explicit VectorF(const boost::numeric::ublas::vector<float>& data); //construct with a ublas::vector<float>
    ~VectorF();
    VectorF(const VectorF& other);
    VectorF& operator=(const VectorF& other);

    void resize(int size, bool to_preserve_data = true); //resize vector
    int size() const; //query size
    const float& at(int idx) const; //element access
    const float& operator[](int idx) const; //operator[]. element access used when VectorF is const obj
    float& operator[](int idx); //operator[] overload. element access used when VectorF is non-const obj

    void set(const boost::numeric::ublas::vector<float>& data); //set data with ublas::vector<float>
    const boost::numeric::ublas::vector<float>& data_const_ref() const; //get underlying ublas::vector<float> data
    boost::numeric::ublas::vector<float>& data(); //get underlying ublas::vector<float> data

    // Overloading the << operator
    friend RRTPLANNER_LIB_EXPORT std::ostream& operator<<(std::ostream& os, const VectorF& obj);
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::VectorF &data);

private:
    QSharedDataPointer<VectorFPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
