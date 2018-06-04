#ifndef ASTARQUADTREE_H_INCLUDED
#define ASTARQUADTREE_H_INCLUDED

#include "QuadTree.h"
#include "ComparatorQuadTree.h"

double heuristic_cost_estimate(Node* start, Node* goal){
    return Distance(start, goal);
}

double dist_between(Node* start, Node* goal){
    return Distance(start, goal);
}

std::vector<Node*> reconstruct_path(std::map<Node*, Node*, ComparatorQuadTree> cameFrom, Node* current){
    std::vector<Node*> total_path{current};
    while(cameFrom.count(current)){
        current = cameFrom[current];
        total_path.push_back(current);
    }
    return total_path;
}

class AstarQuadTree{
private:

    QuadTree* quadtree;

public:

    AstarQuadTree(QuadTree* quadtree) : quadtree(quadtree){}

    std::vector<Node*> FindPath(Point start, Point finish);
    double FindDistance(Point start, Point finish);
    double FindDistance(std::vector<Node*> nodes, Point start, Point finish);
    void CreateFullMatlabPlot(std::string path, Point start, Point finish);
};

std::vector<Node*> AstarQuadTree::FindPath(Point s, Point f){
    std::set<Node*, ComparatorQuadTree> closedSet;
    std::set<Node*, ComparatorQuadTree> openSet;
    Node* start{quadtree -> FindNode(s)};
    Node* goal{quadtree -> FindNode(f)};
    openSet.insert(start);
    std::map<Node*, Node*, ComparatorQuadTree> cameFrom;
    std::map<Node*, double, ComparatorQuadTree> gScore;
    gScore[start] = 0;
    std::map<Node*, double, ComparatorQuadTree> fScore;
    fScore[start] = heuristic_cost_estimate(start, goal);
    while(!openSet.empty()){
            /*
        std::cout << m << " ";
        for(Node* node : openSet){
            node -> Write();
            std::cout << std::endl;
        }
        m++;
        */
        Node* current;
        double minimum{std::numeric_limits<double>::infinity()};
        for(Node* node : openSet){
            if(fScore.count(node)){
                if(minimum > fScore[node]){
                    minimum = fScore[node];
                    current = node;
                }
            }
        }
        if(current == goal)
            return reconstruct_path(cameFrom, current);
        openSet.erase(current);
        closedSet.insert(current);
        std::vector<Node*> neighbors{quadtree -> FindAdjacentNoOccupied(current)};
        for(Node* neighbor : neighbors){
            double tentative_gScore{};
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
    return {};
}

double AstarQuadTree::FindDistance(Point start, Point finish){
    double distance{};
    std::vector<Node*> nodes{FindPath(start, finish)};
    for(int i{}; i<nodes.size()-1; i++){
        distance += Distance(nodes[i], nodes[i+1]);
    }
    distance += sqrt(pow(nodes[0] -> GetCenter().x - start.x, 2) + pow(nodes[0] -> GetCenter().y - start.y, 2));
    distance += sqrt(pow(nodes[nodes.size()] -> GetCenter().x - finish.x, 2) + pow(nodes[nodes.size()] -> GetCenter().y - finish.y, 2));
    return distance;
}

double AstarQuadTree::FindDistance(std::vector<Node*> nodes, Point start, Point finish){
    if(!nodes.size())
        return -1;
    double distance{};
    for(int i{}; i<nodes.size()-1; i++){
        distance += Distance(nodes[i], nodes[i+1]);

    }
    distance += sqrt(pow(nodes[0] -> GetCenter().x - start.x, 2) + pow(nodes[0] -> GetCenter().y - start.y, 2));
    distance += sqrt(pow(nodes[nodes.size()-1] -> GetCenter().x - finish.x, 2) + pow(nodes[nodes.size()-1] -> GetCenter().y - finish.y, 2));

    return distance;
}

void AstarQuadTree::CreateFullMatlabPlot(std::string path, Point start, Point finish){
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

#endif // ASTARQUADTREE_H_INCLUDED
