/**
 * @VectorF.h
 * Wrapper for the underlying boost::numeric::ublas::vector<double> structure used to store coordinates or
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
    explicit VectorF(const std::initializer_list<double>& list); //construct with a list of double values
    explicit VectorF(const boost::numeric::ublas::vector<double>& data); //construct with a ublas::vector<double>
    ~VectorF();
    VectorF(const VectorF& other);
    VectorF& operator=(const VectorF& other);

    void resize(int size, bool to_preserve_data = true); //resize vector
    int size() const; //query size
    const double& at(int idx) const; //element access
    const double& operator[](int idx) const; //operator[]. element access used when VectorF is const obj
    double& operator[](int idx); //operator[] overload. element access used when VectorF is non-const obj

    void set(const boost::numeric::ublas::vector<double>& data); //set data with ublas::vector<double>
    const boost::numeric::ublas::vector<double>& data_const_ref() const; //get underlying ublas::vector<double> data
    boost::numeric::ublas::vector<double>& data(); //get underlying ublas::vector<double> data

    // Overloading the << operator
    friend RRTPLANNER_LIB_EXPORT std::ostream& operator<<(std::ostream& os, const VectorF& obj);
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::VectorF &data);

    QSharedDataPointer<VectorFPrivate> getMp_pimpl() const;
    void setMp_pimpl(QSharedDataPointer<VectorFPrivate> newMp_pimpl);

signals:
    void mp_pimplChanged();

private:
    QSharedDataPointer<VectorFPrivate> mp_pimpl;
    Q_PROPERTY(QSharedDataPointer<VectorFPrivate> mp_pimpl READ getMp_pimpl WRITE setMp_pimpl NOTIFY mp_pimplChanged)
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
