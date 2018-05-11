#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

struct Point{
    double x;
    double y;
    Point(double x, double y) : x(x), y(y) {}

    bool operator==(Point point){
        return x == point.x && y == point.y;
    }
};

#endif // POINT_H_INCLUDED
