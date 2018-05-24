#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

#include "QuadTree.h"

class Astar{
    private:

    QuadTree* quadtree;

    public:

    Astar(QuadTree* quadtree) : quadtree(quadtree){}

    std::vector<Node*> FindPath(Point start, Point finish);

};

std::vector<Node*> Astar::FindPath(Point s, Point f){
    //int i = 0;
    Node* start{quadtree -> FindNode(s)};
    Node* finish{quadtree -> FindNode(f)};
    quadtree -> SetHeuristics(finish);
    std::set<Node*, Comparator> nodes;
    Node* help{start};
    nodes.insert(help);
    while(!nodes.empty()){
        /*
        i++;
        for(Node* pomocna : nodes){
            std::cout << i << ": ";
            pomocna -> Write();
        }
        */
        help = *nodes.begin();
        nodes.erase(nodes.begin());
        help -> cameFrom = help -> maybeCameFrom;
        if(help == finish){
            std::vector<Node*> path;
            path.push_back(help);
            while(help -> cameFrom){
                help -> Write();
                help = help -> cameFrom;
                path.push_back(help);
            }
            return path;
        }
        std::vector<Node*> pom{quadtree -> FindAdjacentNoOccupied(help)};
        for(Node *pomocna : pom){
            if(pomocna == start)
                continue;
            else if(pomocna -> cameFrom)
                continue;

            if(pomocna -> SetWeight(help)){
                /*
                std::cout << "Node: " << "(" << pomocna -> topLeft.x << " " << pomocna -> topLeft.y << "), (" << pomocna -> botRight.x << " " << pomocna -> botRight.y << ")" << std::endl;
                std::cout << "Came from: " << "(" << help -> topLeft.x << " " << help -> topLeft.y << "), (" << help -> botRight.x << " " << help -> botRight.y << ")" << std::endl;
                */
                nodes.insert(pomocna);
            }
        }
    }
    return {};
}

#endif // ASTAR_H_INCLUDED
