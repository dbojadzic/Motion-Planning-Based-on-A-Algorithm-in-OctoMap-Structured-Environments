//
// Created by MrTheamir on 25.06.2018..
//

#ifndef MATRIXV2_ASTAR_H
#define MATRIXV2_ASTAR_H

#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "Point.h"

double heuristic_cost_estimate(const Point &start, const Point &goal){
    return sqrt(pow(goal.x - start.x, 2) + pow(goal.y - start.y, 2));
}

double dist_between(const Point &start, const Point &goal){
    return sqrt(pow(goal.x - start.x, 2) + pow(goal.y - start.y, 2));
}

std::vector<Point> reconstruct_path(Point current, std::vector<Point> &closedSet){
    std::vector<Point> total_path{current};
    std::cout << "zuu";
    Point cameFrom = current.CameFrom();
    for(std::vector<Point>::reverse_iterator iter = closedSet.rbegin(); iter!=closedSet.rend(); iter++){
        if(cameFrom == *iter){
            total_path.push_back(cameFrom);
            cameFrom = iter -> CameFrom();
        }
    }
    return total_path;
}

class Astar{
private:

    int* matrix;
    int width;
    int height;

public:

    Astar(int* matrix, int width, int height) : matrix(matrix), width(width), height(height) {}

    std::vector<Point> FindPath(Point start, Point finish) const ;
    double FindDistance(Point start, Point finish);
    double FindDistance(std::vector<Point> nodes, Point start, Point finish);
    void CreateFullMatlabPlot(std::string path, Point start, Point finish) const ;

};

std::vector<Point> Astar::FindPath(Point start, Point goal) const {
    if(!matrix[start.y*height + start.x])
        return {};
    if(!matrix[goal.y*height + goal.x])
        return {};

    std::vector<Point> closedSet;
    std::set<Point, Point> openSet;
    start.gScore = 0;
    start.fScore = heuristic_cost_estimate(start, goal);
    openSet.insert(start);
    Point current(0,0);
    Point neighbor(0,0);
    while(!openSet.empty()){
        current = *openSet.begin();
        if(current == goal) {
            return reconstruct_path(current, closedSet);
        }

        openSet.erase(openSet.begin());
        closedSet.push_back(current);
        int leftx{current.x - 1}, rightx{current.x + 1}, lefty{current.y - 1}, righty{current.y + 1};

        for(int i{leftx}; i <= rightx; i++){
            if(i < 0 || i >= width)
                continue;
            for(int j{lefty}; j <= righty  ; j++){
                if(j < 0 || j >= height)
                    continue;
                if(i==current.x && j==current.y)
                    continue;
                if(!matrix[j*height + i])
                    continue;
                neighbor = Point(i,j);
                if(std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end())
                    continue;

                double tentative_gScore{current.gScore + dist_between(current, neighbor)};
                if(tentative_gScore >= neighbor.gScore) {
                    openSet.insert(neighbor);
                    continue;
                }
                neighbor.CameFrom(current);
                neighbor.gScore = tentative_gScore;
                neighbor.fScore = tentative_gScore + heuristic_cost_estimate(neighbor, goal);
                openSet.insert(neighbor);
            }
        }
    }
    return {};
}

void Astar::CreateFullMatlabPlot(std::string path, Point start, Point finish) const {
    std::stringstream instructions;
    instructions << "axis([0 " << width << " " << -1 * height << " 0]);" << std::endl;
    instructions << "title('Matrix: " << width << " x " << height << " From: (" << start.x << "," << start.y << ") To: (" << finish.x << "," << finish.y << ")');" << std::endl;
    instructions << "xlabel('Width: " << width << "');" << std::endl;
    instructions << "ylabel('Height: " << height << "');" << std::endl;
    std::vector<Point> points{FindPath(start, finish)};
    instructions << "X=[";
    instructions << finish.x << ",";
    for(Point point : points){
        instructions << point.x << ",";
    }
    instructions << start.x;
    instructions << "];" << std::endl;
    instructions << "Y=[";
    instructions << -finish.y << ",";
    for(Point point : points){
        instructions << point.y * -1 << ",";
    }
    instructions << -start.y;
    instructions << "];" << std::endl;
    instructions << "hold on;" << std::endl;
    instructions << "plot(X,Y,'r');" << std::endl;
    //instructions << "legend('" << FindDistance(nodes, start, finish) << "','Location','southoutside');" << std::endl;
    std::ofstream write(path);
    write << instructions.str();

}

#endif //MATRIXV2_ASTAR_H
