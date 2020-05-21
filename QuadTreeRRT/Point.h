#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "Pose.h"

struct Point{
    double x{};
    double y{};
    Point() = default;
    Point(double x, double y) : x(x), y(y) {}
    Point(Pose p) : x(p.x), y(p.y) {};

    bool operator==(Point point) const {
        return x == point.x && y == point.y;
    }
};

#endif // POINT_H_INCLUDED
