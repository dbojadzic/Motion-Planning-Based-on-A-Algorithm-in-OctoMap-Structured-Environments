#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

#include "QuadTree.h"

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
    Node* start{quadtree -> FindNode(s)};
    Node* finish{quadtree -> FindNode(f)};
    quadtree -> SetHeuristics(finish);
    std::set<Node*, Comparator> nodes;
    Node* help{start};
    nodes.insert(help);
    while(!nodes.empty()){
        help = *nodes.begin();
        nodes.erase(nodes.begin());
        help -> cameFrom = help -> maybeCameFrom;
        if(help == finish){
            std::vector<Node*> path;
            path.push_back(help);
            while(help -> cameFrom){
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
                nodes.insert(pomocna);
            }
        }
    }
    return {};
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

void Astar::CreateFullMatlabPlot(std::string path, Point start, Point finish){
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

#endif // ASTAR_H_INCLUDED
