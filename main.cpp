#include <iostream>
#include <stdexcept>
#include <random>
#include <time.h>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

#include "QuadTree.h"

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
        //std::cout << x << " " << y << std::endl;
        if(!matrix[y*width + x])
            matrix[y*width + x] = 1;
        else
            i--;
    }
}

void generateFullMatrix(int matrix[], int width, int height){
    for(int i{}; i<width*height; i++){
            matrix[i] = 1;
    }
}

void loadMatrix(std::string ime, int matrix[], int width, int height){
    std::ifstream file;
    file.open(ime);
    char a;
    int i = 0;
    while(!file.eof()){
        file >> a;
        if(a != ' '){
            matrix[i] = a - '0';
            i++;
        }
    }
}

void generateTxt(std::string ime, int matrix[], int width, int height){
    std::ofstream izlaz(ime);
    for(int i{}; i<width; i++){
        for(int j{}; j<height; j++)
            izlaz << matrix[j*width + i];
        izlaz << std::endl;
    }
}

bool compareFiles(std::string prvi, std::string drugi){
    std::ifstream file1, file2;
    file1.open(prvi);
    file2.open(drugi);
    char a,b;
    while(!file1.eof() && !file2.eof()){
        file1 >> a;
        file2 >> b;
        if(a != b)
            return false;
    }
    return true;
}

void setDepth(int width, int height, double tolerance){
    DEPTH = 2;
    while(width/2. > tolerance || height/2 > tolerance){
        width/=2;
        height/=2;
        DEPTH++;
    }
}

int main(){
    QuadTree dumir(Point(0,0), Point(64,64));
    dumir.InsertPoint(Point(1,1));
    Node* help{dumir.FindNode(Point(3,3))};
    std::cout << "Node: (" << help -> topLeft.x << ", " << help -> topLeft.y << ") (" << help -> botRight.x << ", " << help -> botRight.y << ")" << std::endl;
    std::vector<Node*> neighbors{dumir.FindAdjacent(help)};
    for(Node* node : neighbors){
        std::cout << "(" << node -> topLeft.x << ", " << node -> topLeft.y << ") (" << node -> botRight.x << ", " << node -> botRight.y << ")" << std::endl;
    }
}


/*
int main(){
    srand(time(NULL));
    int width = 100;
    int height = 100;
    for(int j = 0; j<6; j++){
        std::stringstream lokacija;
        lokacija << "reports/Izvjestaj";
        lokacija << width;
        lokacija << "x";
        lokacija << height;
        lokacija << ".txt";
        std::ofstream izvjestaj(lokacija.str());
        int timer = 0;
        int noder = 0;
        for(int i = 0; i<50; i++){
            int* matrix = new int[width*height]{};
            generateMatrix(matrix, width, height);
            clock_t vrijeme1 = clock();
            QuadTree dumir(matrix, width, height);
            clock_t vrijeme2 = clock();
            int nodes = dumir.countNodes();
            izvjestaj << j << ". "<< width << " " << height << " " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms. " << nodes << " nodes" << std::endl;
            timer += (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            noder += nodes;
            delete[] matrix;
        }
        izvjestaj << timer << std::endl;
        izvjestaj << noder;
        height *= 2;
        width *= 2;
    }

    width = 100;
    height = 100;
    for(int j = 0; j<10; j++){
        std::stringstream lokacija;
        lokacija << "reports/Izvjestaj2";
        lokacija << width;
        lokacija << "x";
        lokacija << height;
        lokacija << ".txt";
        std::ofstream izvjestaj(lokacija.str());
        int timer = 0;
        int noder = 0;
        for(int i = 0; i<50; i++){
            int* matrix = new int[width*height]{};
            generateMatrix(matrix, width, height);
            clock_t vrijeme1 = clock();
            QuadTree dumir(matrix, width, height);
            clock_t vrijeme2 = clock();
            int nodes = dumir.countNodes();
            izvjestaj << j << ". "<< width << " " << height << " " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms. " << nodes << " nodes" << std::endl;
            timer += (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            noder += nodes;
            delete[] matrix;
        }
        izvjestaj << timer << std::endl;
        izvjestaj << noder;
        width *= 2;
    }
    return 0;
}
*/
/*
int main() //main za generisanje izvještaja i poreðenje matrica
{
    srand(time(NULL));
    std::ofstream izvjestaj("izvjestajehlimanayes.txt");
    int width = 6400;
    int height = 100;
    for(int i{}; i<3; i++){
        //setDepth(width, height, 1);
        for(int j{}; j<5; j++){
            std::cout << i << " " << j << " ";
            int* matrix = new int[width*height]{};
            //generateMatrix(matrix, width, height);
            generateFullMatrix(matrix, width, height);
            clock_t vrijeme1 = clock();
            QuadTree dumir(matrix, width, height);
            clock_t vrijeme2 = clock();
            izvjestaj << j << ". "<< width << " " << height << " " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms. " << dumir.countNodes() << " nodes" << std::endl;

            std::stringstream lokacija;
            lokacija << "Matrica";
            lokacija << width;
            lokacija << "x";
            lokacija << height;
            lokacija << "x";
            lokacija << j;
            lokacija << ".txt";
            generateTxt(lokacija.str(), matrix, width, height);

            std::stringstream lokacija2;
            lokacija2 << "QuadTree";
            lokacija2 << width;
            lokacija2 << "x";
            lokacija2 << height;
            lokacija2 << "x";
            lokacija2 << j;
            lokacija2 << ".txt";
            std::ofstream ispis(lokacija2.str());

            for(int i = 0; i < width; i++){
                for(int j = 0; j<height; j++){
                    ispis << dumir.isOccupied(Point(i,j));
                }
                ispis << std::endl;
            }

            std::stringstream lokacija3;
            lokacija3 << "DepthQuadTree";
            lokacija3 << width;
            lokacija3 << "x";
            lokacija3 << height;
            lokacija3 << "x";
            lokacija3 << j;
            lokacija3 << ".txt";
            std::ofstream ispis2(lokacija3.str());
            for(int i = 0; i < width; i++){
                for(int j = 0; j<height; j++){
                    ispis2 << dumir.getDepth(Point(i,j));
                }
                ispis2 << std::endl;
            }
            delete[] matrix;
            std::cout << compareFiles(lokacija.str(), lokacija2.str()) << std::endl;
        }
        width *= 2;
        //height *= 2;
    }
    return 0;
}
*/

