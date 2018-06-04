#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

#include <set>
#include <map>

#include "Node.h"
#include "QuadTree.h"
#include "Comparator.h"

double heuristic_cost_estimate(Node* start, Node* goal){
    return Distance(start, goal);
}

double dist_between(Node* start, Node* goal){
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

/*
std::vector<Node*> reconstruct_path(std::map<Node*, Node*, Comparator> cameFrom, Node* current){
    std::vector<Node*> total_path{current};
    while(cameFrom.count(current)){
        current = cameFrom[current];
        total_path.push_back(current);
    }
    return total_path;
}
*/

class Astar{
private:

    QuadTree* quadtree;

public:

    Astar(QuadTree* quadtree) : quadtree(quadtree){}

    std::vector<Node*> FindPath(Point start, Point finish);
    double FindDistance(Point start, Point finish);
    double FindDistance(std::vector<Node*> nodes, Point start, Point finish);
    void CreateFullMatlabPlot(std::string path, Point start, Point finish);
};

std::vector<Node*> Astar::FindPath(Point s, Point f){

    std::set<Node*> closedSet;
    std::set<Node*, Comparator> openSet;
    Node* start{quadtree -> FindNode(s)};
    Node* goal{quadtree -> FindNode(f)};
    openSet.insert(start);
    start -> gScore = 0;
    start -> fScore = heuristic_cost_estimate(start, goal);
    int m = 0;
    while(!openSet.empty()){
        /*
        std::cout << std::endl;
        std::cout << m << " ";
        for(Node* node : openSet){
            node -> Write();
            std::cout << std::endl;
        }
        m++;
        */
        Node* current{*openSet.begin()};
        /*
        std::cout << std::endl;
        std::cout << "Current: " << std::endl;
        current -> Write();
        */
        if(current == goal)
            return reconstruct_path(current);


        openSet.erase(openSet.begin());
        closedSet.insert(current);

        std::vector<Node*> neighbors{quadtree -> FindAdjacentNoOccupied(current)};
        for(Node* neighbor : neighbors){
            /*
            std::cout << "neighbor: " << std::endl;
            neighbor -> Write();
            std::cout << std::endl;

            for(Node* node : closedSet){
                node -> Write();
                std::cout << std::endl;
            }
            */

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

void Astar::CreateFullMatlabPlot(std::string path, Point start, Point finish){

    std::stringstream instructions;
    instructions << "axis([0 " << quadtree -> GetWidth() << " " <<-1 * quadtree -> GetHeight() << " 0]);" << std::endl;
    instructions << "title('QuadTree: " << quadtree -> GetWidth() << " x " << quadtree -> GetHeight() << " From: (" << start.x << "," << start.y << ") To: (" << finish.x << "," << finish.y << ")');" << std::endl;
    instructions << "xlabel('Width: " << quadtree -> GetWidth() << "');" << std::endl;
    instructions << "ylabel('Height: " << quadtree -> GetHeight() << "');" << std::endl;
    quadtree -> StreamMapMatlab(instructions);
    std::vector<Node*> nodes{FindPath(start, finish)};
std::cout <<"Hi";
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

double Astar::FindDistance(Point start, Point finish){
    double distance{};
    std::vector<Node*> nodes{FindPath(start, finish)};
    for(int i{}; i<nodes.size()-1; i++){
        distance += Distance(nodes[i], nodes[i+1]);
    }
    distance += sqrt(pow(nodes[0] -> GetCenter().x - start.x, 2) + pow(nodes[0] -> GetCenter().y - start.y, 2));
    distance += sqrt(pow(nodes[nodes.size()] -> GetCenter().x - finish.x, 2) + pow(nodes[nodes.size()] -> GetCenter().y - finish.y, 2));
    return distance;
}

double Astar::FindDistance(std::vector<Node*> nodes, Point start, Point finish){
    double distance{};
    for(int i{}; i<nodes.size()-1; i++){
        distance += Distance(nodes[i], nodes[i+1]);

    }
    distance += sqrt(pow(nodes[0] -> GetCenter().x - start.x, 2) + pow(nodes[0] -> GetCenter().y - start.y, 2));
    distance += sqrt(pow(nodes[nodes.size()-1] -> GetCenter().x - finish.x, 2) + pow(nodes[nodes.size()-1] -> GetCenter().y - finish.y, 2));

    return distance;
}

#endif // ASTAR_H_INCLUDED
