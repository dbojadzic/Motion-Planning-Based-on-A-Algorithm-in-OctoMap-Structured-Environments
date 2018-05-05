#include <iostream>
#include <stdexcept>
#include <random>
#include <time.h>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>

double DEPTH = 1;
int PERCENTAGE = 10;


struct Point{
    double x;
    double y;
    Point(double x, double y) : x(x), y(y) {}
};

struct Node{
    Point topLeft;
    Point botRight;
    bool occupied = false;
    Node* topLeftTree = nullptr;
    Node* topRightTree = nullptr;
    Node* botLeftTree = nullptr;
    Node* botRightTree = nullptr;

    Node(Point topLeft, Point botRight, bool occupied = false) : topLeft(topLeft), botRight(botRight), occupied(occupied) {

    }

    ~Node(){
        delete topLeftTree;
        delete topRightTree;
        delete botLeftTree;
        delete botRightTree;
    }

    int Count(){
        if(topLeftTree)
            return topLeftTree -> Count() + topRightTree -> Count() + botLeftTree -> Count() + botRightTree -> Count() + 1;
        else
            return 1;
    }
};

class QuadTree{
    private:
    Node* root = nullptr;
    Point topLeft;
    Point botRight;

    public:
    QuadTree(Point topLeft, Point botRight) : topLeft(topLeft), botRight(botRight){
        root = new Node(topLeft, botRight);
    }

    QuadTree(int matrix[], int width, int height);

    ~QuadTree(){
        delete root;
    }

    void insertPoint(Point point);
    bool isOccupied(Point point);
    int countNodes();
};

QuadTree::QuadTree(int matrix[], int width, int height) : topLeft(0, 0), botRight(width, height){
    root = new Node(topLeft, botRight);
    for(int i{}; i<width; i++){
        for (int j{}; j<height; j++){
            if(matrix[i*width + j]){
                insertPoint(Point(i,j));
            }
        }
    }
}

void QuadTree::insertPoint(Point point){
    if(point.x < topLeft.x || point.x > botRight.x || point.y < topLeft.y || point.y > botRight.y)
        throw std::range_error("Out of bounds");
    Node* current = root;
    do{
        //std::cout << current->topLeft.x << " " << current->topLeft.y << " " << current->botRight.x << " " << current->botRight.y << std::endl;
        if(!current -> topLeftTree){
            current -> topLeftTree  = new Node(current -> topLeft,                                                                                        Point((current -> topLeft.x + current -> botRight.x)/2, (current -> topLeft.y + current -> botRight.y)/2));
            current -> topRightTree = new Node(Point((current -> topLeft.x + current -> botRight.x)/2, current -> topLeft.y),                             Point(current -> botRight.x, (current -> topLeft.y + current -> botRight.y)/2));
            current -> botLeftTree  = new Node(Point(current -> topLeft.x, (current -> topLeft.y + current -> botRight.y)/2),                             Point((current -> topLeft.x + current -> botRight.x)/2, current -> botRight.y));
            current -> botRightTree = new Node(Point((current -> topLeft.x + current -> botRight.x)/2, (current -> topLeft.y + current -> botRight.y)/2), current -> botRight);
        }

        if(point.x < (current -> topLeft.x + current -> botRight.x)/2 && point.y < (current -> topLeft.y + current -> botRight.y)/2){
            current = current -> topLeftTree;
        }
        else if(point.x >= (current -> topLeft.x + current -> botRight.x)/2 && point.y < (current -> topLeft.y + current -> botRight.y)/2){
            current = current -> topRightTree;
        }
        else if(point.x < (current -> topLeft.x + current -> botRight.x)/2 && point.y >= (current -> topLeft.y + current -> botRight.y)/2){
            current = current -> botLeftTree;
        }
        else{
            current = current -> botRightTree;
        }
    }while(current -> botRight.x - current -> topLeft.x > DEPTH && current -> botRight.y - current -> topLeft.y > DEPTH);
    current -> occupied = true;
}

bool QuadTree::isOccupied(Point point){
    if(point.x < topLeft.x || point.x > botRight.x || point.y < topLeft.y || point.y > botRight.y)
        throw std::range_error("Out of bounds");
    Node* current = root;
    while(current -> topLeftTree){
        if(point.x < (current -> topLeft.x + current -> botRight.x)/2 && point.y < (current -> topLeft.y + current -> botRight.y)/2){
            current = current -> topLeftTree;
        }
        else if(point.x >= (current -> topLeft.x + current -> botRight.x)/2 && point.y < (current -> topLeft.y + current -> botRight.y)/2){
            current = current -> topRightTree;
        }
        else if(point.x < (current -> topLeft.x + current -> botRight.x)/2 && point.y >= (current -> topLeft.y + current -> botRight.y)/2){
            current = current -> botLeftTree;
        }
        else{
            current = current -> botRightTree;
        }
    }
    return current -> occupied;
}

int QuadTree::countNodes(){
    return root -> Count();
}

/************************************/

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
        if(!matrix[x*width + y])
            matrix[x*width + y] = 1;
        else
            i--;
    }
}

void generateTxt(std::string ime, int matrix[], int width, int height){
    std::ofstream izlaz(ime);
    for(int i{}; i<width; i++){
        for(int j{}; j<height; j++)
            izlaz << matrix[i*width + j];
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

int main()
{
    srand(time(NULL));
    std::ofstream izvjestaj("izvjestaj.txt");
    int width = 100;
    int height = 100;
    for(int i{}; i<10; i++){
        for(int j{}; j<50; j++){
            std::cout << i << " " << j << " ";
            int* matrix = new int[width*height]{};
            generateMatrix(matrix, width, height);

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
            delete[] matrix;
            std::cout << compareFiles(lokacija.str(), lokacija2.str()) << std::endl;
        }
        width *= 2;
        height *= 2;
    }
    return 0;
}


