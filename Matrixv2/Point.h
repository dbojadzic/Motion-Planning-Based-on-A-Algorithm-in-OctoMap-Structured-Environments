//
// Created by MrTheamir on 25.06.2018..
//

#ifndef MATRIXV2_POINT_H
#define MATRIXV2_POINT_H

#include <limits>

struct Point{
    int x{};
    int y{};

    double fScore = std::numeric_limits<double>::infinity();
    double gScore = std::numeric_limits<double>::infinity();

    int old_x{-1};
    int old_y{-1};

    Point() = default;

    Point(int x, int y) : x(x), y(y) {}

    Point CameFrom() const {
        return {old_x, old_y};
    }

    void CameFrom(const Point &p){
        old_x = p.x;
        old_y = p.y;
    }

    bool operator==(Point point) const {
        return x == point.x && y == point.y;
    }
    /*
    Point &operator =(Point p){
        x = p.x;
        y = p.y;
        fScore = p.fScore;
        gScore = p.gScore;
        old_x = p.old_x;
        old_y = p.old_y;
    }
    */
    bool operator()(Point a, Point b) const {
        if(a==b) {
            return false;
        }
        else if (a.fScore == b.fScore) {
            return a.x * 10000 + a.y < b.x * 10000 + b.y;
        }

        return a.fScore < b.fScore;
    }
};

#endif //MATRIXV2_POINT_H
