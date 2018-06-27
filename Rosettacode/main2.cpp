#include <list>
#include <algorithm>
#include <iostream>

#include "point.h"
#include "map.h"
#include "aStar.h"

int PERCENTAGE = 10;

inline int SlucajniBroj (int mini, int maxi){
    int n = maxi - mini + 1;
    int ost = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - ost);
    return mini + x % n;
}

void generateMatrix(int matrix[], int width, int height){
    int occupied = width * height / PERCENTAGE;
    for(int i{}; i<occupied; i++){
        int x = SlucajniBroj(0, width-1);
        int y = SlucajniBroj(0, height-1);
        if(matrix[y*width + x])
            matrix[y*width + x] = 0;
        else
            i--;
    }
}

void generateFullMatrix(int matrix[], int width, int height){
    for(int i{}; i<width*height; i++){
        matrix[i] = 1;
    }
}

int main( int argc, char* argv[] ) {
    point s(1,1), e( 99,99 );


    int width = 100;
    int height = 100;
    int* matrix = new int[width*height]{};
    generateFullMatrix(matrix, width, height);
    generateMatrix(matrix, width, height);
    map *m = new map(width, height, matrix);

    aStar as(m);


    if( as.search( s, e ) ) {
        std::list<point> path;
        int c = as.path( path );
        for( int y = -1; y < 101; y++ ) {
            for( int x = -1; x < 101; x++ ) {
                if( x < 0 || y < 0 || x > 100 || y > 100 || (*m)( x, y ) == 1 )
                    std::cout << char(0xdb);
                else {
                    if( std::find( path.begin(), path.end(), point( x, y ) )!= path.end() )
                        std::cout << "x";
                    else std::cout << ".";
                }
            }
            std::cout << "\n";
        }

        std::cout << "\nPath cost " << c << ": ";
        for( std::list<point>::iterator i = path.begin(); i != path.end(); i++ ) {
            std::cout<< "(" << ( *i ).x << ", " << ( *i ).y << ") ";
        }
    }

    std::cout << "\n\n";
    delete m;
    return 0;
}