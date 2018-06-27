//
// Created by MrTheamir on 26.06.2018..
//

#ifndef ROSETTACODE_POINT_H
#define ROSETTACODE_POINT_H

class point {
public:
    int x, y;

    point( int a = 0, int b = 0 ) { x = a; y = b; }
    bool operator ==( const point& o ) { return o.x == x && o.y == y; }
    point operator +( const point& o ) { return {o.x + x, o.y + y}; }
};

#endif //ROSETTACODE_POINT_H
