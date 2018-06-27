//
// Created by MrTheamir on 26.06.2018..
//

#ifndef ROSETTACODE_MAP_H
#define ROSETTACODE_MAP_H

class map {
public:
    int *matrix;
    int w, h;

    map(int w, int h) : w(w), h(h){
        int* matrix = new int[w*h]{};
    }

    map(int w, int h, int *matrix) : w(w), h(h), matrix(matrix) {}

    ~map(){
        delete[] matrix;
    }

    int operator() ( int x, int y ) { return matrix[x*h + y]; }

};

#endif //ROSETTACODE_MAP_H
