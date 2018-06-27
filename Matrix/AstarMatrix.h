#ifndef ASTARMATRIX_H_INCLUDED
#define ASTARMATRIX_H_INCLUDED

#include <map>
#include <c++/4.8.3/algorithm>
#include "Point.h"

double heuristic_cost_estimate(const Point &start, const Point &goal){
    return sqrt(pow(goal.x - start.x, 2) + pow(goal.y - start.y, 2));
}

double dist_between(const Point &start, const Point &goal){
    return sqrt(pow(goal.x - start.x, 2) + pow(goal.y - start.y, 2));
}

std::vector<Point> reconstruct_path(std::map<Point, Point, Point> &cameFrom, Point current){
    std::vector<Point> total_path{current};
    while(cameFrom.count(current)){
        current = cameFrom[current];
        total_path.push_back(current);
    }
    return total_path;
}

class AstarMatrix{
private:

    int* matrix;
    int width;
    int height;

public:

    AstarMatrix(int* matrix, int width, int height) : matrix(matrix), width(width), height(height) {}

    std::vector<Point> FindPath(Point start, Point finish) const ;
    double FindDistance(Point start, Point finish);
    double FindDistance(std::vector<Point> nodes, Point start, Point finish) const;
    void CreateFullMatlabPlot(std::string path, Point start, Point finish) const;

};


std::vector<Point> AstarMatrix::FindPath(Point start, Point goal) const {
    if(!matrix[(int)(start.y*height + start.x)])
        return {};
     if(!matrix[(int)(goal.y*height + goal.x)])
        return {};
    std::set<Point, Point> closedSet;
    std::set<Point, Point> openSet;
    openSet.insert(start);
    std::map<Point, Point, Point> cameFrom;
    std::map<Point, double, Point> gScore;
    gScore[start] = 0;
    std::map<Point, double, Point> fScore;
    fScore[start] = heuristic_cost_estimate(start, goal);
    while(!openSet.empty()){
        Point current{*std::min_element(openSet.begin(), openSet.end(), [&fScore](const Point &a, const Point &b){
            return fScore[a] < fScore[b];
        })};
        if(current == goal)
            return reconstruct_path(cameFrom, current);
        openSet.erase(current);
        closedSet.insert(current);
        int leftx{current.x - 1}, rightx{current.x + 1}, lefty{current.y - 1}, righty{current.y + 1};
        for(int i{leftx}; i <= rightx; i++){
            if(i < 0 || i >= width)
                continue;
            for(int j{lefty}; j <= righty ; j++){
                if(j < 0 || j >= height)
                    continue;
                if(i==current.x && j==current.y)
                    continue;
                if(!matrix[j*height + i])
                    continue;

                Point neighbor(i,j);
                if(closedSet.count(neighbor))
                    continue;
                openSet.insert(neighbor);
                double tentative_gScore{};
                if(!gScore.count(current))
                    gScore[current] = std::numeric_limits<double>::infinity();
                if(!gScore.count(neighbor))
                    gScore[neighbor] = std::numeric_limits<double>::infinity();
                tentative_gScore = gScore[current] + dist_between(current, neighbor);
                if(tentative_gScore >= gScore[neighbor])
                    continue;
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + heuristic_cost_estimate(neighbor, goal);
            }
        }
    }
    return {};
}

/*
std::vector<Point> AstarMatrix::FindPath(Point start, Point goal) const {
    if(!matrix[(int)(start.y*height + start.x)])
        return {};
    if(!matrix[(int)(goal.y*height + goal.x)])
        return {};
    std::set<Point, Point> closedSet;
    std::set<Point, Point> openSet;
    openSet.insert(start);
    std::map<Point, Point, Point> cameFrom;
    std::map<Point, double, Point> gScore;
    gScore[start] = 0;
    std::map<Point, double, Point> fScore;
    fScore[start] = heuristic_cost_estimate(start, goal);
    while(!openSet.empty()){
        Point current;
        double minimum{std::numeric_limits<double>::infinity()};
        for(Point point : openSet){
            if(fScore.count(point)){
                if(minimum > fScore[point]){
                    minimum = fScore[point];
                    current = point;
                }
            }
        }
        if(current == goal)
            return reconstruct_path(cameFrom, current);
        openSet.erase(current);
        closedSet.insert(current);
        for(int i{current.x - 1}; i <= current.x + 1; i++){
            if(i < 0 || i >= width)
                continue;
            for(int j{current.y - 1}; j <= current.y + 1 ; j++){
                if(j < 0 || j >= height)
                    continue;
                if(i==current.x && j==current.y)
                    continue;
                if(!matrix[j*height + i])
                    continue;

                Point neighbor(i,j);
                if(closedSet.count(neighbor))
                    continue;
                openSet.insert(neighbor);
                double tentative_gScore{};
                if(!gScore.count(current))
                    gScore[current] = std::numeric_limits<double>::infinity();
                if(!gScore.count(neighbor))
                    gScore[neighbor] = std::numeric_limits<double>::infinity();
                tentative_gScore = gScore[current] + dist_between(current, neighbor);
                if(tentative_gScore >= gScore[neighbor])
                    continue;
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + heuristic_cost_estimate(neighbor, goal);
            }
        }
    }
    return {};
}
 */

void AstarMatrix::CreateFullMatlabPlot(std::string path, Point start, Point finish) const {
    std::stringstream instructions;
    instructions << "A=[";
    for(int i{}; i<width; i++){
        for(int j{}; j<height; j++){
            instructions << matrix[i*height + j] << " ";
        }
        instructions << ";" << std::endl;
    }
    instructions << "];" << std::endl;
    instructions << "imshow(A);" << std::endl;
    instructions << "axis on" << std::endl;
    instructions << "hold on" << std::endl;
    instructions << "title('Matrix: " << width << " x " << height << " From: (" << start.x << "," << start.y << ") To: (" << finish.x << "," << finish.y << ")');" << std::endl;
    instructions << "xlabel('Width: " << width << "');" << std::endl;
    instructions << "ylabel('Height: " << height << "');" << std::endl;
    std::vector<Point> points{FindPath(start, finish)};
    instructions << "X=[";
    instructions << finish.x + 1 << ",";
    for(Point point : points){
        instructions << point.x + 1 << ",";
    }
    instructions << start.x + 1;
    instructions << "];" << std::endl;
    instructions << "Y=-[";
    instructions << -finish.y - 1<< ",";
    for(Point point : points){
        instructions << point.y * -1 - 1<< ",";
    }
    instructions << -start.y - 1;
    instructions << "];" << std::endl;
    instructions << "hold on;" << std::endl;
    instructions << "plot(X,Y,'r');" << std::endl;
    instructions << "legend('" << FindDistance(points, start, finish) << "','Location','southoutside');" << std::endl;
    std::ofstream write(path);
    write << instructions.str();
}

double AstarMatrix::FindDistance(std::vector<Point> nodes, Point start, Point goal) const {
    if(!nodes.size())
        return -1;
    double distance{};
    for(int i{}; i<nodes.size()-1; i++){
        distance += sqrt(pow(nodes[i+1].x - nodes[i].x, 2) + pow(nodes[i+1].y  - nodes[i].y, 2));
    }
    distance += sqrt(pow(nodes[0].x - start.x, 2) + pow(nodes[0].y  - start.y, 2));
    distance += sqrt(pow(nodes[nodes.size()-1].x - goal.x, 2) + pow(nodes[nodes.size()-1].y  - goal.y, 2));
    return distance;
}

#endif // ASTARMATRIX_H_INCLUDED
