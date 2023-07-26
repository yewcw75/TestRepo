#include <RrtPlannerLib/framework/algorithm/gjk/internal/SimplexBasic.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QVector>


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

//function aliases for readability here
static const auto& dot = VectorFHelper::dot_product;
static const auto& subtract_vector = VectorFHelper::subtract_vector;
static const auto& multiply = VectorFHelper::multiply_value;
static const auto& cross_product_zVal = VectorFHelper::cross_product_zVal;

class SimplexBasicPrivate
{
public:
    SimplexBasicPrivate(Simplex* parent)
        :mp_parent(parent)
    {}
    SimplexBasicPrivate(const SimplexBasicPrivate& other) = delete; //non-copyable
    ~SimplexBasicPrivate() = default;

    bool handle2D(VectorF& v);
    bool handle1D(VectorF& v);
    bool handle0D(VectorF& v);

public:
    Simplex* mp_parent{};
    QVector<VectorF> m_vertexList;
};

//----------
bool SimplexBasicPrivate::handle2D(VectorF& v)
{
    bool originInSimplex{false}; //for returning
    v = VectorF{0.0, 0.0}; //init for returning

    //search direction
    const VectorF& c  = m_vertexList.at(0);
    const VectorF& b  = m_vertexList.at(1);
    const VectorF& a  = m_vertexList.at(2);
    VectorF ab = subtract_vector(b, a);
    VectorF ac = subtract_vector(c, a);
    VectorF ao = multiply(a, -1.0);

    //qInfo() << "a: " << a << ", b: " << b << ", c: " << c;
    //qInfo() << "ab: " << ab << ", ac: " << ac << ", ao: " << ao;

    double zVal = ab.at(0)*ac.at(1) - ab.at(1)*ac.at(0); //z-value of ab cross ac
    VectorF ac_perp = multiply(VectorF{-ac.at(1), ac.at(0)}, zVal);
    VectorF ab_perp = multiply(VectorF{ab.at(1), -ab.at(0)}, zVal);

    //qInfo() << "zVal: " << zVal;
    //qInfo() << "ac_perp: " << ac_perp << ", ab_perp: " << ab_perp;

    //----------------------
    //check origin on edge ac and ab
    double ao_dot_ac_perp = dot(ao, ac_perp);
    double ao_dot_ab_perp = dot(ao, ab_perp);
    double ac_perp_norm_square = dot(ac_perp, ac_perp);
    double ab_perp_norm_square = dot(ab_perp, ab_perp);
    originInSimplex = ao_dot_ac_perp*ao_dot_ac_perp < mp_parent->eps_square() * ac_perp_norm_square || \
            ao_dot_ab_perp * ao_dot_ab_perp < mp_parent->eps_square() * ab_perp_norm_square;

    //----------------------
    //check for region RAC and RAB
    if(!originInSimplex){
        if (dot(ac_perp, ao) > 0){ //in region RAC
            if (dot(ac, ao) > 0){
                m_vertexList.pop_back(); //discard 3rd vertex
                m_vertexList[0] = c;
                m_vertexList[1] = a;
            }
            else {
                m_vertexList.pop_back(); //discard 3rd vertex
                m_vertexList.pop_back(); //discard 2rd vertex
                m_vertexList[0] = a;
            }
            v = ac_perp; //Update search direction
        }
        else if (dot(ab_perp, ao) > 0){ //in region RAB
            if (dot(ab, ao) > 0) {
                m_vertexList.pop_back(); //discard 3rd vertex
                m_vertexList[0]  = b;
                m_vertexList[1]  = a;
            }
            else {
                m_vertexList.pop_back(); //discard 3rd vertex
                m_vertexList.pop_back(); //discard 2rd vertex
                m_vertexList[0]  = a;
            }
            v = ab_perp; //Update search direction
        }
        else { //neither in RAC nor RAB
            //Update simplex
            VectorF cb = subtract_vector(b, c);
            VectorF ba = subtract_vector(a, b);
            VectorF bo = multiply(b, -1.0);

            m_vertexList[0]  = c;
            m_vertexList[1]  = b;
            m_vertexList[2]  = a;

            originInSimplex = true;
        }
    } //if(!originInSimplex)
    return(originInSimplex);
}

//----------
bool SimplexBasicPrivate::handle1D(VectorF& v)
{
    const VectorF& b = m_vertexList.at(0);
    const VectorF& a = m_vertexList.at(1);

    VectorF ab = subtract_vector(b, a);
    VectorF ao =  multiply(a, -1.0);

    //Update search direction
    double scalar = ao.at(0)*ab.at(1) - ao.at(1)*ab.at(0);
    v = multiply(VectorF{ab.at(1), -ab.at(0)}, scalar);

    //check origin on line AB
    double ao_dot_v = dot(ao, v);
    double v_dot_v = dot(v, v);
    bool originInSimplex = (ao_dot_v * ao_dot_v) < (mp_parent->eps_square() * v_dot_v);
    return(originInSimplex);
}

//----------
bool SimplexBasicPrivate::handle0D(VectorF& v)
{
    v = multiply(m_vertexList.at(0), -1.0);
    return(false);
}

//####################
SimplexBasic::SimplexBasic()
    :Simplex(),
     d_ptr(new SimplexBasicPrivate(this))
{

}

//----------
SimplexBasic::SimplexBasic(double eps_square)
    :Simplex(eps_square),
     d_ptr(new SimplexBasicPrivate(this))
{

}

//----------
SimplexBasic::~SimplexBasic()
{

}

//----------
void SimplexBasic::reset()
{
    d_ptr->m_vertexList.clear();
}

//----------
bool SimplexBasic::update(const VectorF& vertex, VectorF& v)
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
        isOriginInSimplex = d_ptr->handle0D(v); //will return v as Origin - vertex[0]
    }
    return(isOriginInSimplex);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
