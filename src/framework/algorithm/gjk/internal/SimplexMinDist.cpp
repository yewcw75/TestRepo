#include <RrtPlannerLib/framework/algorithm/gjk/internal/SimplexMinDist.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <QVector>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

//function aliases for readability here
static const auto& dot = VectorFHelper::dot_product;
static const auto& subtract_vector = VectorFHelper::subtract_vector;
static const auto& multiply = VectorFHelper::multiply_value;
static const auto& cross_product_zVal = VectorFHelper::cross_product_zVal;

class SimplexMinDistPrivate
{
public:
    SimplexMinDistPrivate(Simplex* parent)
        :mp_parent(parent)
    {}
    SimplexMinDistPrivate(const SimplexMinDistPrivate& other) = delete; //non-copyable
    ~SimplexMinDistPrivate() = default;

    bool handle2D(VectorF& v);
    bool handle1D(VectorF& v);
    bool handle0D(VectorF& v);

public:
    Simplex* mp_parent{};
    QVector<VectorF> m_vertexList;
};

//----------
bool SimplexMinDistPrivate::handle2D(VectorF& v)
{
    bool originInSimplex{false}; //for returning
    v.resize(m_vertexList.at(0).size(), false); //init for returning

    //search direction
    VectorF oc  = m_vertexList.at(0);
    VectorF ob  = m_vertexList.at(1);
    VectorF oa  = m_vertexList.at(2);
    VectorF ab = subtract_vector(ob, oa);
    VectorF ca = subtract_vector(oa, oc);
    VectorF ao = multiply(oa, -1.0);

    double z_val = cross_product_zVal(ca, ab); //z value of ca x ab (up-vector)
    double sign_z = z_val < 0.0 ? -1.0 : 1.0;

    VectorF ab_perp = multiply( VectorFHelper::vec2D_cross_z(ab), z_val); //ab x (ca x ab)
    VectorF ca_perp = multiply( VectorFHelper::vec2D_cross_z(ca), z_val); //ca x (ca x ab)

    double up_ca = dot(oa, ca); //u'_ca
    double vp_ca = dot(multiply(oc, -1.0), ca); //v'_ca (used for assert only)
    double up_ab = dot(ob, ab); //u'_ab (used for assert only)
    double vp_ab = dot(ao, ab); //v'_ca

    double wp_ab = cross_product_zVal(oa, ob) * sign_z; //z value of oa X ob
    double wp_ca = cross_product_zVal(oc, oa) * sign_z; //z value of oc X oa
    double wp_bc = cross_product_zVal(ob, oc) * sign_z; //z value of ob X oc (used for assert only)

    //function lambdas
    auto dot_square = [](VectorF a, VectorF b) {
        auto val = dot(a, b);
        return(val*val);
    };

    //Handle different cases
    if (up_ca <= 0 && vp_ab <=0){ //Case 1: Origin in RA
        Q_ASSERT(vp_ca > 0);
        Q_ASSERT(up_ab > 0);
        v = ao;

        //Remove b and c
        m_vertexList.pop_front(); //pop c
        m_vertexList.pop_front(); //pop b
    }
    else if (dot_square(ao, ab_perp) < mp_parent->eps_square() * dot(ab_perp, ab_perp)) {//Case 2A: Origin on line AB
        originInSimplex = true;
    }
    else if (wp_ab < 0 && vp_ab > 0) {//Case 2B: Origin in RAB exclude line AB
        Q_ASSERT(up_ab > 0);

        double ab_dot_ab = dot(ab, ab);
        Q_ASSERT(ab_dot_ab > TOL_SMALL);
        v = subtract_vector(ao, multiply(ab, vp_ab/ab_dot_ab));

        //Remove c
        m_vertexList.pop_front();
    }
    else if (dot_square(oa, ca_perp) < mp_parent->eps_square() * dot(ca_perp, ca_perp)) {//Case 3A: Origin on line CA
        originInSimplex = true;
    }
    else if (wp_ca < 0 && up_ca > 0) {//Case 3B: Origin in RCA exclude line CA
        Q_ASSERT(vp_ca > 0);

        double ca_dot_ca = dot(ca, ca);
        Q_ASSERT(ca_dot_ca > TOL_SMALL);
        v = subtract_vector(multiply(ca, up_ca/ca_dot_ca), oa);

        //Remove b
        m_vertexList.remove(1);
    }
    else{
        Q_ASSERT(wp_ab > 0);
        Q_ASSERT(wp_bc > 0);
        Q_ASSERT(wp_ca > 0);
        originInSimplex = true;
    }
    return(originInSimplex);
}

//----------
bool SimplexMinDistPrivate::handle1D(VectorF& v)
{
    bool originInSimplex{false};

    const VectorF& b = m_vertexList.at(0);
    const VectorF& a = m_vertexList.at(1);

    VectorF ab = subtract_vector(b, a);
    VectorF ao =  multiply(a, -1.0);
    double ao_dot_ab = dot(ao, ab);

    if(ao_dot_ab <= 0.0) { //case 1: Origin in RA
        m_vertexList.pop_front(); //remove b
        v = ao;
    }
    else{ //case 2: Origin in RAB
        double ab_dot_ab = dot(ab, ab);
        double s = ao_dot_ab/ab_dot_ab;
        v = subtract_vector(ao, multiply(ab, s));

        //check origin on line AB
        double v_dot_v = dot(v, v);
        originInSimplex = v_dot_v < mp_parent->eps_square();
    }
    return(originInSimplex);
}

//----------
bool SimplexMinDistPrivate::handle0D(VectorF& v)
{
    v = multiply(m_vertexList.at(0), -1.0);
    return(false);
}

//####################
SimplexMinDist::SimplexMinDist()
    :Simplex(),
      d_ptr(new SimplexMinDistPrivate(this))
{

}

//----------
SimplexMinDist::SimplexMinDist(double eps_square)
    :Simplex(eps_square),
      d_ptr(new SimplexMinDistPrivate(this))
{

}

//----------
SimplexMinDist::~SimplexMinDist()
{

}

//----------
void SimplexMinDist::reset()
{
    d_ptr->m_vertexList.clear();
}

//----------
bool SimplexMinDist::update(const VectorF& vertex, VectorF& v)
{
    bool isOriginInSimplex{false};
    d_ptr->m_vertexList.append(vertex);
    switch(d_ptr->m_vertexList.size()){
    case 3:
        isOriginInSimplex = d_ptr->handle2D(v);
        break;
    case 2:
        isOriginInSimplex = d_ptr->handle1D(v);
        break;
    default:
        isOriginInSimplex = d_ptr->handle0D(v);
    }
    return(isOriginInSimplex);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
