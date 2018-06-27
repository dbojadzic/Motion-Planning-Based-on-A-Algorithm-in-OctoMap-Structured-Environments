//
// Created by MrTheamir on 26.06.2018..
//

#ifndef ROSETTACODE_NODE_H
#define ROSETTACODE_NODE_H

#include "point.h"

class node {
public:
    point pos, parent;
    int dist, cost;
    bool operator == (const node& o ) { return pos == o.pos; }
    bool operator == (const point& o ) { return pos == o; }
    bool operator < (const node& o ) { return dist + cost < o.dist + o.cost; }
};

#endif //ROSETTACODE_NODE_H
