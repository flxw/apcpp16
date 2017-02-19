
#include "BezierCurve.h"


namespace
{


std::pair<float, float> lerp(const std::pair<float, float> & p0, const std::pair<float, float> & p1, float t)
{
    return {
        p0.first * (1.0f - t) + p1.first * t,
        p0.second * (1.0f - t) + p1.second * t
    };
}


} // unnamed namespace


BezierCurve::BezierCurve(const Point & a, const Point & b, const Point & c, const Point & d)
: m_a{ a }
, m_b{ b }
, m_c{ c }
, m_d{ d }
{
}


std::pair<float, float> BezierCurve::operator()(float t) const
{
    const auto ab = lerp(m_a, m_b, t);
    const auto bc = lerp(m_b, m_c, t);
    const auto cd = lerp(m_c, m_d, t);

    const auto abbc = lerp(ab, bc, t);
    const auto bccd = lerp(bc, cd, t);

    return lerp(abbc, bccd, t);
}
