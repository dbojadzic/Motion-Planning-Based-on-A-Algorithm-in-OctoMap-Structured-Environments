#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

#include <set>
#include <map>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

#include "Node.h"
#include "QuadTree.h"
#include "Comparator.h"

double heuristic_cost_estimate(const Node* start, const Node* goal){
    return Distance(start, goal);
}

double dist_between(const Node* start, const Node* goal){
    return Distance(start, goal);
}


std::vector<Node*> reconstruct_path(Node* current){
    std::vector<Node*> total_path{current};
    while(current -> cameFrom){
        current = current -> cameFrom;
        total_path.push_back(current);
    }
    return total_path;
}

class Astar{
private:

    QuadTree* quadtree;

public:

    Astar(QuadTree* quadtree) : quadtree(quadtree){}

    std::vector<Node*> FindPath(Point start, Point finish);
    std::vector<Node*> FindPath(Point start, Point finish, int &nodes);
    double FindDistance(Point start, Point finish);
    double FindDistance(std::vector<Node*> nodes, Point start, Point finish);
    void CreateFullMatlabPlot(const std::string &path, Point start, Point finish);
    int CreateFullMatlabPlotTest(const std::string &path, Point start, Point finish);
};

std::vector<Node*> Astar::FindPath(Point s, Point f){
    if(quadtree -> IsOccupied(s) || quadtree -> IsOccupied(f)){
        return {};
    }
    quadtree -> Reset();
    std::set<Node*> closedSet;
    std::set<Node*, Comparator> openSet;
    Node* start{quadtree -> FindNode(s)};
    Node* goal{quadtree -> FindNode(f)};
    //std::cout << start->GetCenter().x << "," << start->GetCenter().y << std::endl;
    openSet.insert(start);
    start -> gScore = 0;
    start -> fScore = heuristic_cost_estimate(start, goal);
    while(!openSet.empty()){
        Node* current{*openSet.begin()};
        if(current == goal)
            return reconstruct_path(current);
        openSet.erase(openSet.begin());
        closedSet.insert(current);
        std::vector<Node*> neighbors{quadtree -> FindAdjacentNoOccupied(current)};
        for(Node* neighbor : neighbors){
            if(closedSet.count(neighbor)){
                continue;
            }
            openSet.insert(neighbor);
            double tentative_gScore{current -> gScore + dist_between(current, neighbor)};
            if(tentative_gScore >= neighbor -> gScore)
                continue;
            neighbor -> cameFrom = current;
            neighbor -> gScore = tentative_gScore;
            neighbor -> fScore = tentative_gScore + heuristic_cost_estimate(neighbor, goal);
        }
    }
    return {};
}

std::vector<Node*> Astar::FindPath(Point s, Point f, int &nodes){
    if(quadtree -> IsOccupied(s) || quadtree -> IsOccupied(f)){
        nodes = -1;
        return {};
    }
    quadtree -> Reset();
    std::set<Node*> closedSet;
    std::set<Node*, Comparator> openSet;
    Node* start{quadtree -> FindNode(s)};
    Node* goal{quadtree -> FindNode(f)};
    openSet.insert(start);
    start -> gScore = 0;
    start -> fScore = heuristic_cost_estimate(start, goal);
    int m = 0;
    while(!openSet.empty()){
        Node* current{*openSet.begin()};
        if(current == goal) {
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
            openSet.insert(neighbor);
            double tentative_gScore{current -> gScore + dist_between(current, neighbor)};
            if(tentative_gScore >= neighbor -> gScore)
                continue;
            neighbor -> cameFrom = current;
            neighbor -> gScore = tentative_gScore;
            neighbor -> fScore = tentative_gScore + heuristic_cost_estimate(neighbor, goal);
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
    std::vector<Node*> nodes{FindPath(start, finish)};
    instructions << "X=[";
    instructions << finish.x << ",";
    for(Node* node : nodes){
        instructions << node -> GetCenter().x << ",";
    }
    instructions << start.x;
    instructions << "];" << std::endl;
    instructions << "Y=[";
    instructions << -finish.y << ",";
    for(Node* node : nodes){
        instructions << node -> GetCenter().y * -1 << ",";
    }
    instructions << -start.y;
    instructions << "];" << std::endl;
    instructions << "hold on;" << std::endl;
    instructions << "plot(X,Y,'r');" << std::endl;
    instructions << "legend('" << FindDistance(nodes, start, finish) << "','Location','southoutside');" << std::endl;
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
    std::vector<Node*> nodes{FindPath(start, finish, nodescount)};
    instructions << "X=[";
    instructions << finish.x << ",";
    for(Node* node : nodes){
        instructions << node -> GetCenter().x << ",";
    }
    instructions << start.x;
    instructions << "];" << std::endl;
    instructions << "Y=[";
    instructions << -finish.y << ",";
    for(Node* node : nodes){
        instructions << node -> GetCenter().y * -1 << ",";
    }
    instructions << -start.y;
    instructions << "];" << std::endl;
    instructions << "hold on;" << std::endl;
    instructions << "plot(X,Y,'r');" << std::endl;
    instructions << "legend('" << FindDistance(nodes, start, finish) << "','Location','southoutside');" << std::endl;
    std::ofstream write(path);
    write << instructions.str();
    return nodescount;
}

double Astar::FindDistance(Point start, Point finish){
    std::vector<Node*> nodes{FindPath(start, finish)};
    return FindDistance(nodes, start, finish);
}

double Astar::FindDistance(std::vector<Node*> nodes, Point start, Point finish){
    if(!nodes.size())
        return 0;
    double distance{};
    for(int i{}; i<nodes.size()-1; i++){
        distance += Distance(nodes[i], nodes[i+1]);

    }
    distance += sqrt(pow(nodes[0] -> GetCenter().x - finish.x, 2) + pow(nodes[0] -> GetCenter().y - finish.y, 2));
    distance += sqrt(pow(nodes[nodes.size()-1] -> GetCenter().x - start.x, 2) + pow(nodes[nodes.size()-1] -> GetCenter().y - start.y, 2));
    return distance;
}

#endif // ASTAR_H_INCLUDED
