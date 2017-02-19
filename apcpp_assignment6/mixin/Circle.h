
#pragma once

#include <utility>


class Circle
{
public:
    explicit Circle(const std::pair<float, float> & center, float radius);
    virtual ~Circle() = default;

    virtual std::pair<float, float> operator()(float t) const;


private:
    std::pair<float, float> m_center;
    float m_radius;
};
