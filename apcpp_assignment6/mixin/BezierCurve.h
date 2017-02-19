
#pragma once


#include <utility>


class BezierCurve
{
public:
    using Point = std::pair<float, float>;


public:
    explicit BezierCurve(const Point & a, const Point & b, const Point & c, const Point & d);
    virtual ~BezierCurve() = default;

    virtual Point operator()(float t) const;


private:
    Point m_a;
    Point m_b;
    Point m_c;
    Point m_d;
};
