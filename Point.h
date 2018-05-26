#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

struct Point{
    double x{};
    double y{};
    Point(){}
    Point(double x, double y) : x(x), y(y) {}

    bool operator==(Point point) const {
        return x == point.x && y == point.y;
    }

    bool operator()(Point a, Point b) const {
        return a.x*1000+a.y < b.x*1000+b.y;
    }
};

#endif // POINT_H_INCLUDED
