#include <RrtPlannerLib/framework/algorithm/gjk/Simplex.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QVector>

#define EPS_REL 1e-6

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SimplexPrivate
{
public:
    SimplexPrivate()
    {}
    ~SimplexPrivate()
    {}

    bool handle2D(VectorF& v);
    bool handle1D(VectorF& v);
    bool handle0D(VectorF& v);

public:
    QVector<VectorF> m_vertexList;
};

//----------
bool SimplexPrivate::handle2D(VectorF& v)
{
    bool originInSimplex{false}; //for returning
    v = VectorF{0.0, 0.0}; //init for returning

    //search direction
    const VectorF& c  = m_vertexList.at(0);
    const VectorF& b  = m_vertexList.at(1);
    const VectorF& a  = m_vertexList.at(2);
    VectorF ab = VectorFHelper::subtract_vector(b, a);
    VectorF ac = VectorFHelper::subtract_vector(c, a);
    VectorF ao = VectorFHelper::multiply_value(a, -1.0);

    //qInfo() << "a: " << a << ", b: " << b << ", c: " << c;
    //qInfo() << "ab: " << ab << ", ac: " << ac << ", ao: " << ao;

    double zVal = ab.at(0)*ac.at(1) - ab.at(1)*ac.at(0); //z-value of ab cross ac
    VectorF ac_perp = VectorFHelper::multiply_value(VectorF{-ac.at(1), ac.at(0)}, zVal);
    VectorF ab_perp = VectorFHelper::multiply_value(VectorF{ab.at(1), -ab.at(0)}, zVal);

    //qInfo() << "zVal: " << zVal;
    //qInfo() << "ac_perp: " << ac_perp << ", ab_perp: " << ab_perp;

    //----------------------
    //check origin on edge ac and ab
    double ao_dot_ac_perp = VectorFHelper::dot_product(ao, ac_perp);
    double ao_dot_ab_perp = VectorFHelper::dot_product(ao, ab_perp);
    double ac_perp_norm_square = VectorFHelper::dot_product(ac_perp, ac_perp);
    double ab_perp_norm_square = VectorFHelper::dot_product(ab_perp, ab_perp);
    originInSimplex = ao_dot_ac_perp*ao_dot_ac_perp < EPS_REL * ac_perp_norm_square || \
            ao_dot_ab_perp * ao_dot_ab_perp < EPS_REL * ab_perp_norm_square;

    //----------------------
    //check for region RAC and RAB
    if(!originInSimplex){
        if (VectorFHelper::dot_product(ac_perp, ao) > 0){ //in region RAC
            if (VectorFHelper::dot_product(ac, ao) > 0){
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
        else if (VectorFHelper::dot_product(ab_perp, ao) > 0){ //in region RAB
            if (VectorFHelper::dot_product(ab, ao) > 0) {
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
            VectorF cb = VectorFHelper::subtract_vector(b, c);
            VectorF ba = VectorFHelper::subtract_vector(a, b);
            VectorF bo = VectorFHelper::multiply_value(b, -1.0);

            m_vertexList[0]  = c;
            m_vertexList[1]  = b;
            m_vertexList[2]  = a;

            originInSimplex = true;
        }
    } //if(!originInSimplex)
    return(originInSimplex);
}

//----------
bool SimplexPrivate::handle1D(VectorF& v)
{
    const VectorF& b = m_vertexList.at(0);
    const VectorF& a = m_vertexList.at(1);

    VectorF ab = VectorFHelper::subtract_vector(b, a);
    VectorF ao =  VectorFHelper::multiply_value(a, -1.0);

    //Update search direction
    double scalar = ao.at(0)*ab.at(1) - ao.at(1)*ab.at(0);
    v = VectorFHelper::multiply_value(VectorF{ab.at(1), -ab.at(0)}, scalar);

    //check origin on line AB
    double ao_dot_v = VectorFHelper::dot_product(ao, v);
    double v_dot_v = VectorFHelper::dot_product(v, v);
    bool originInSimplex = (ao_dot_v * ao_dot_v) < (EPS_REL * v_dot_v);
    return(originInSimplex);
}

//----------
bool SimplexPrivate::handle0D(VectorF& v)
{
    v = VectorFHelper::multiply_value(m_vertexList.at(0), -1.0);
    return(false);
}

//####################
Simplex::Simplex()
    :mp_pimpl(new SimplexPrivate())
{

}

//----------
Simplex::~Simplex()
{

}

bool Simplex::update(const VectorF& vertex, VectorF& v)
{
    bool isOriginInSimplex{false};
    mp_pimpl->m_vertexList.append(vertex);
    switch(mp_pimpl->m_vertexList.size()){
    case 3:
        isOriginInSimplex = mp_pimpl->handle2D(v);
        break;
    case 2:
        isOriginInSimplex = mp_pimpl->handle1D(v);
        break;
    default:
        isOriginInSimplex = mp_pimpl->handle0D(v); //will return v as Origin - vertex[0]
    }
    return(isOriginInSimplex);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
