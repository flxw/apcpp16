
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <cmath>

#include "BezierCurve.h"
#include "Circle.h"
#include "Cached.h"


using Canvas = std::array<std::array<char, 100>, 50>;


template <typename Type>
void drawImpl(const Type & object, Canvas & canvas)
{
    for (auto t = 0.0f; t <= 1.0f; t += 1.0f / 256.0f)
    {
        const auto point = object(t);

        const auto x = std::lround(point.first);
        const auto y = std::lround(point.second);

        canvas[y][x] = 'X';
    }
}


void drawBezierCurve(const BezierCurve & curve, Canvas & canvas)
{
    drawImpl(curve, canvas);
}


void drawCircle(const Circle & circle, Canvas & canvas)
{
    drawImpl(circle, canvas);
}


void write(const Canvas & image, const std::string & filename)
{
    auto output = std::ofstream{ filename };
    if (!output)
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    for (const auto & line : image)
    {
        output.write(line.data(), line.size());
        output.put('\n');
    }
}


int main()
{
    auto image = Canvas{};
    for (auto & line : image)
    {
        std::fill(line.begin(), line.end(), ' ');
    }

    auto curve = Cached<BezierCurve> {
        {  0.0f,  0.0f },
        { 15.0f, 48.0f },
        { 73.0f,  7.0f },
        { 99.0f, 49.0f }
    };

    auto circle1 = Cached<Circle> { { 80.0f, 14.0f }, 3.2f };
    auto circle2 = Cached<Circle> { { 18.0f, 37.0f }, 4.7f };

    curve.setCacheResolution(32);
    circle1.setCacheResolution(7);
    circle2.setCacheResolution(7);
    circle2.setUseLinearInterpolation(true);

    drawBezierCurve(curve, image);
    drawCircle(circle1, image);
    drawCircle(circle2, image);
    write(image, "low_res.txt");


    for (auto & line : image)
    {
        std::fill(line.begin(), line.end(), ' ');
    }

    curve.setCacheResolution(256);
    circle1.setCacheResolution(256);
    circle2.setCacheResolution(256);

    drawBezierCurve(curve, image);
    drawCircle(circle1, image);
    drawCircle(circle2, image);
    write(image, "high_res.txt");
}
