#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

#include <set>
#include <map>
#include <algorithm>

#include "Node.h"
#include "QuadTree.h"
#include "Comparator.h"

double heuristic_cost_estimate(const Node* start, const Node* goal){
    return Distance(start, goal);
}

double dist_between(const Node* start, const Node* goal){
    return Distance(start, goal);
}

double point_distance(const Point &a, const Point &b){
    return sqrt(pow(b.x-a.x,2) + pow(b.y-a.y,2));
}
double point_distance(const Point &a, const Point &b, const Point &c){
    return point_distance(a,b) + point_distance(b,c);
}

std::vector<Point> reconstruct_path(Node* current){
    std::vector<Point> total_path{current->AccessPoint};
    while(current -> cameFrom){
        current = current -> cameFrom;
        total_path.push_back(current->AccessPoint);
    }
    return total_path;
}
/*
std::vector<Point> reconstruct_path(Node* current){
    std::vector<Point> total_path{current->GetCenter()};
    while(current -> cameFrom){
        current = current -> cameFrom;
        total_path.push_back(current->GetCenter());
    }
    return total_path;
}
*/
/*
std::vector<Point> reconstruct_path(Node* current){
    std::vector<Point> total_path{current->BestPoint, current->AccessPoint};
    while(current -> cameFrom){
        current = current -> cameFrom;
        total_path.push_back(current->BestPoint);
        total_path.push_back(current->AccessPoint);
    }
    return total_path;
}
 */

class Astar{
private:

    QuadTree* quadtree;

public:

    Astar(QuadTree* quadtree) : quadtree(quadtree){}

    std::vector<Point> FindPath(Point s, Point f);
    std::vector<Point> FindPath(Point s, Point f, int &nodes);
    double FindDistance(Point start, Point finish);
    double FindDistance(std::vector<Point> nodes, Point start, Point finish) const ;
    void CreateFullMatlabPlot(const std::string &path, Point start, Point finish);
    int CreateFullMatlabPlotTest(const std::string &path, Point start, Point finish);
};

std::vector<Point> Astar::FindPath(Point s, Point f){
    if(quadtree -> IsOccupied(s) || quadtree -> IsOccupied(f)){
        return {};
    }
    quadtree -> Reset();
    std::set<Node*> closedSet;
    std::set<Node*, Comparator> openSet;
    Node* start{quadtree -> FindNode(s)};
    Node* goal{quadtree -> FindNode(f)};
    start -> AccessPoint = s;
    start -> gScore = 0;
    start -> fScore = point_distance(start -> AccessPoint, f);
    openSet.insert(start);
    while(!openSet.empty()){
        Node* current{*openSet.begin()};
        if(current == goal) {
            //current -> BestPoint = f;
            return reconstruct_path(current);
        }
        openSet.erase(openSet.begin());
        closedSet.insert(current);
        std::vector<Node*> neighbors{quadtree -> FindAdjacentNoOccupied(current)};
        for(Node* neighbor : neighbors){
            if(closedSet.count(neighbor)){
                continue;
            }

            std::vector<Point> points = neighbor -> GetBorderPoints();
            std::vector<Point> borders = current -> GetBorder(neighbor);

            neighbor -> PossibleBestPoint = *std::min_element(points.begin(), points.end(), [current, f, borders](const Point &a, const Point &b){
                return point_distance(current->AccessPoint, a) + point_distance(a, f) < point_distance(current->AccessPoint, b) + point_distance(b, f);
            });

            neighbor -> PossibleAccessPoint = *std::min_element(borders.begin(), borders.end(), [current, neighbor](const Point &a, const Point &b){
                return point_distance(current->AccessPoint, a, neighbor->PossibleBestPoint)  <  point_distance(current->AccessPoint, b, neighbor->PossibleBestPoint);
            });

            double tentative_gScore{current -> gScore + point_distance(current->AccessPoint, neighbor->PossibleAccessPoint)};
            if(tentative_gScore >= neighbor -> gScore) {
                openSet.insert(neighbor);
                continue;
            }
            neighbor -> cameFrom = current;
            neighbor -> AccessPoint = neighbor -> PossibleAccessPoint;
            neighbor -> BestPoint = neighbor -> PossibleBestPoint;
            neighbor -> gScore = tentative_gScore;
            neighbor -> fScore = tentative_gScore + point_distance(neighbor->AccessPoint, f);
            openSet.insert(neighbor);
        }
    }
    return {};
}

std::vector<Point> Astar::FindPath(Point s, Point f, int &nodes){
    if(quadtree -> IsOccupied(s) || quadtree -> IsOccupied(f)){
        nodes = -1;
        return {};
    }
    quadtree -> Reset();
    std::set<Node*> closedSet;
    std::set<Node*, Comparator> openSet;
    Node* start{quadtree -> FindNode(s)};
    Node* goal{quadtree -> FindNode(f)};
    start -> AccessPoint = s;
    start -> gScore = 0;
    start -> fScore = point_distance(start -> AccessPoint, f);
    openSet.insert(start);
    while(!openSet.empty()){
        Node* current{*openSet.begin()};
        if(current == goal) {
            //current -> BestPoint = f;
            nodes = openSet.size() + closedSet.size();
            return reconstruct_path(current);
        }
        openSet.erase(openSet.begin());
        closedSet.insert(current);
        std::vector<Node*> neighbors{quadtree -> FindAdjacentNoOccupied(current)};
        for(Node* neighbor : neighbors){
            if(closedSet.count(neighbor)){
                continue;
            }

            std::vector<Point> points = neighbor -> GetBorderPoints();
            std::vector<Point> borders = current -> GetBorder(neighbor);

            neighbor -> PossibleBestPoint = *std::min_element(points.begin(), points.end(), [current, f, borders](const Point &a, const Point &b){
                /*
                Point helpA = *std::min_element(borders.begin(), borders.end(), [current, a](const Point &c, const Point &d){
                    return point_distance(current->AccessPoint, c, a)  <  point_distance(current->AccessPoint, d, a);
                });
                Point helpB = *std::min_element(borders.begin(), borders.end(), [current, b](const Point &c, const Point &d){
                    return point_distance(current->AccessPoint, c, b)  <  point_distance(current->AccessPoint, d, b);
                });
                return point_distance(current->AccessPoint, helpA) + point_distance(helpA, a) + point_distance(a, f) < point_distance(current->AccessPoint, helpB) + point_distance(helpB, b) + point_distance(b, f);
                */
                return point_distance(current->AccessPoint, a) + point_distance(a, f) < point_distance(current->AccessPoint, b) + point_distance(b, f);
            });

            neighbor -> PossibleAccessPoint = *std::min_element(borders.begin(), borders.end(), [current, neighbor](const Point &a, const Point &b){
                return point_distance(current->AccessPoint, a, neighbor->PossibleBestPoint)  <  point_distance(current->AccessPoint, b, neighbor->PossibleBestPoint);
            });

            double tentative_gScore{current -> gScore + point_distance(current->AccessPoint, neighbor->PossibleAccessPoint)};
            if(tentative_gScore >= neighbor -> gScore) {
                openSet.insert(neighbor);
                continue;
            }
            neighbor -> cameFrom = current;
            neighbor -> AccessPoint = neighbor -> PossibleAccessPoint;
            neighbor -> BestPoint = neighbor -> PossibleBestPoint;
            neighbor -> gScore = tentative_gScore;
            neighbor -> fScore = tentative_gScore + point_distance(neighbor->AccessPoint, f);
            openSet.insert(neighbor);
        }
    }
    nodes = -1;
    return {};
}

void Astar::CreateFullMatlabPlot(const std::string &path, Point start, Point finish){
    std::stringstream instructions;
    instructions << "axis([0 " << quadtree -> GetWidth() << " " <<-1 * quadtree -> GetHeight() << " 0]);" << std::endl;
    instructions << "title('QuadTree: " << quadtree -> GetWidth() << " x " << quadtree -> GetHeight() << " From: (" << start.x << "," << start.y << ") To: (" << finish.x << "," << finish.y << ")');" << std::endl;
    instructions << "xlabel('Width: " << quadtree -> GetWidth() << "');" << std::endl;
    instructions << "ylabel('Height: " << quadtree -> GetHeight() << "');" << std::endl;
    quadtree -> StreamMapMatlab(instructions);
    std::vector<Point> points{FindPath(start, finish)};
    instructions << "X=[";
    instructions << finish.x  << ",";
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
    instructions << "plot(X,Y,'bo');" << std::endl;
    instructions << "legend('" << FindDistance(points, start, finish) << "','Location','southoutside');" << std::endl;
    std::ofstream write(path);
    write << instructions.str();
}

int Astar::CreateFullMatlabPlotTest(const std::string &path, Point start, Point finish){

    std::stringstream instructions;
    instructions << "axis([0 " << quadtree -> GetWidth() << " " <<-1 * quadtree -> GetHeight() << " 0]);" << std::endl;
    instructions << "title('QuadTree: " << quadtree -> GetWidth() << " x " << quadtree -> GetHeight() << " From: (" << start.x << "," << start.y << ") To: (" << finish.x << "," << finish.y << ")');" << std::endl;
    instructions << "xlabel('Width: " << quadtree -> GetWidth() << "');" << std::endl;
    instructions << "ylabel('Height: " << quadtree -> GetHeight() << "');" << std::endl;
    quadtree -> StreamMapMatlab(instructions);
    int nodescount{};
    std::vector<Point> points{FindPath(start, finish, nodescount)};
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
    instructions << -start.y ;
    instructions << "];" << std::endl;
    instructions << "hold on;" << std::endl;
    instructions << "plot(X,Y,'r');" << std::endl;
    instructions << "plot(X,Y,'bo');" << std::endl;
    instructions << "legend('" << FindDistance(points, start, finish) << "','Location','southoutside');" << std::endl;
    std::ofstream write(path);
    write << instructions.str();
    return nodescount;
}

double Astar::FindDistance(Point start, Point goal){
    std::vector<Point> nodes{FindPath(start, goal)};
    return FindDistance(nodes, start, goal);
}

double Astar::FindDistance(std::vector<Point> nodes, Point start, Point goal) const {
    if(!nodes.size())
        return -1;
    double distance{};
    for(int i{}; i<nodes.size()-1; i++){
        distance += sqrt(pow(nodes[i+1].x - nodes[i].x, 2) + pow(nodes[i+1].y  - nodes[i].y, 2));
    }
    distance += sqrt(pow(nodes[0].x - goal.x, 2) + pow(nodes[0].y  - goal.y, 2));
    distance += sqrt(pow(nodes[nodes.size()-1].x - start.x, 2) + pow(nodes[nodes.size()-1].y  - start.y, 2));
    return distance;
}

#endif // ASTAR_H_INCLUDED
