#include <iostream>
#include <fstream>
#include <chrono>

#include "Point.h"
#include "Astar.h"

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

void loadMatrix(const std::string &ime, int matrix[], int width, int height){
    std::ifstream file;
    file.open(ime);
    char a;
    int i = 0;
    file >> a;
    while(!file.eof()){
        if(a != ' ' && a != ','){
            matrix[i] = a - '0';
            i++;
        }
        file >> a;
    }
}

void generateTxt(std::string ime, int matrix[], int width, int height){
    std::ofstream izlaz(ime);
    for(int i{}; i<width; i++){
        for(int j{}; j<height; j++)
            izlaz << matrix[j * width + i] << ",";
        izlaz << std::endl;
    }
}

int main(){
    int width = 1000;
    int height = 1000;
    int* matrix = new int[width*height]{};
    //loadMatrix("maps/B1500.txt", matrix, width, height);
    generateFullMatrix(matrix, width, height);
    generateMatrix(matrix, width, height);
    Astar dumir(matrix, width, height);
    auto start = std::chrono::high_resolution_clock::now();
    dumir.FindPath(Point(0, 0), Point(999, 999));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << elapsed.count();
    //dumir.CreateFullMatlabPlot("Kemalinstr.m", Point(1, 1), Point(500, 500));
    generateTxt("Kemal.txt", matrix, width, height);
    delete[] matrix;
    return 0;
}