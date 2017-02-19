
#include "Circle.h"

#include <cmath>


namespace
{


const auto PI = 3.14159265359f;


} // unnamed namespace


Circle::Circle(const std::pair<float, float> & center, float radius)
: m_center{ center }
, m_radius{ radius }
{
}


std::pair<float, float> Circle::operator()(float t) const
{
    return {
        m_center.first + m_radius * std::cos(t * 2 * PI),
        m_center.second + m_radius * std::sin(t * 2 * PI),
    };
}
