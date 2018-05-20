#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

#include <set>

#include "Point.h"
#include "Node.h"
#include "Position.h"
#include "Comparator.h"

int DEPTH = 6;

class QuadTree{
    private:
    Node* root = nullptr;
    Point topLeft;
    Point botRight;

    public:
    QuadTree(Point topLeft, Point botRight) : topLeft(topLeft), botRight(botRight){
        root = new Node(nullptr, topLeft, botRight, 0);
    }

    QuadTree(int matrix[], int width, int height);

    ~QuadTree(){
        delete root;
    }

    void InsertPoint(Point point);
    bool IsOccupied(Point point);
    Node* FindNode(Point point);
    int GetDepth(Point point);
    int CountNodes();
    std::vector<Node*> FindAdjacent(Node* node);
    void SetHeuristics(Node* referent);
    std::vector<Node*> Astar(Node* start, Node* finish);
    std::vector<Node*> FindAdjacentNoOccupied(Node* node);
    void WriteAll(){
        root -> WriteAll();
    }
};

QuadTree::QuadTree(int matrix[], int width, int height) : topLeft(0, 0), botRight(width, height){
    root = new Node(nullptr, topLeft, botRight, 0);
    for(int i{}; i<width; i++){
        for (int j{}; j<height; j++){
            if(matrix[j*width + i]){
                InsertPoint(Point(i,j));
            }
        }
    }
}

void QuadTree::InsertPoint(Point point){
    if(point.x < topLeft.x || point.x > botRight.x || point.y < topLeft.y || point.y > botRight.y)
        throw std::range_error("Out of bounds");
    Node* current = root;
    while(!current->occupied && current->depth != DEPTH){
        //std::cout << current->topLeft.x << " " << current->topLeft.y << " " << current->botRight.x << " " << current->botRight.y << std::endl;
        if(!current -> occupied && !current -> topLeftTree){
            current -> topLeftTree  = new Node(current, current -> topLeft,                                                                                        Point((current -> topLeft.x + current -> botRight.x)/2, (current -> topLeft.y + current -> botRight.y)/2), current -> depth +1);
            current -> topRightTree = new Node(current, Point((current -> topLeft.x + current -> botRight.x)/2, current -> topLeft.y),                             Point(current -> botRight.x, (current -> topLeft.y + current -> botRight.y)/2), current -> depth +1);
            current -> botLeftTree  = new Node(current, Point(current -> topLeft.x, (current -> topLeft.y + current -> botRight.y)/2),                             Point((current -> topLeft.x + current -> botRight.x)/2, current -> botRight.y), current -> depth +1);
            current -> botRightTree = new Node(current, Point((current -> topLeft.x + current -> botRight.x)/2, (current -> topLeft.y + current -> botRight.y)/2), current -> botRight, current -> depth +1);
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
        else if(point.x >= (current -> topLeft.x + current -> botRight.x)/2 && point.y >= (current -> topLeft.y + current -> botRight.y)/2)
        {
            current = current -> botRightTree;
        }
        else{
            throw std::range_error("Invalid division");
        }
    }
    current -> occupied = true;
    if(current -> parent)
        current -> parent -> Unite();
}

bool QuadTree::IsOccupied(Point point){
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

Node* QuadTree::FindNode(Point point){
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
    return current;
}

int QuadTree::GetDepth(Point point){
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
    return current -> depth;
}

int QuadTree::CountNodes(){
    return root -> Count();
}

std::vector<Node*> QuadTree::FindAdjacent(Node* node){
    std::vector<Node*> adjecent;
    root -> FindBordering(adjecent, node -> topLeft, node -> botRight);
    return adjecent;
}

std::vector<Node*> QuadTree::FindAdjacentNoOccupied(Node* node){
    std::vector<Node*> adjecent;
    root -> FindBorderingNoOccupied(adjecent, node -> topLeft, node -> botRight);
    return adjecent;
}

void QuadTree::SetHeuristics(Node* referent){
    root -> SetHeuristics(referent);
}

std::vector<Node*> QuadTree::Astar(Node* start, Node* finish){
    root -> SetHeuristics(finish);
    std::set<Node*, Comparator> nodes;
    Node* help{start};
    nodes.insert(help);
    while(!nodes.empty()){
        help = *nodes.begin();
        nodes.erase(nodes.begin());
        if(help == finish){
            std::vector<Node*> path;
            path.push_back(help);
            while(help -> cameFrom){
                std::cout << "(" << help -> topLeft.x << " " << help -> topLeft.y << "), (" << help -> botRight.x << " " << help -> botRight.y << ")" << std::endl;
                help = help -> cameFrom;
                path.push_back(help);
            }
            return path;
        }
        std::vector<Node*> pom{FindAdjacentNoOccupied(help)};
        for(Node *pomocna : pom){
            if(pomocna == start)
                continue;
            else if(pomocna -> cameFrom == help)
                continue;
            if(pomocna -> SetWeight(help)){
                std::cout << "Node: " << "(" << pomocna -> topLeft.x << " " << pomocna -> topLeft.y << "), (" << pomocna -> botRight.x << " " << pomocna -> botRight.y << ")" << std::endl;
                std::cout << "Came from: " << "(" << help -> topLeft.x << " " << help -> topLeft.y << "), (" << help -> botRight.x << " " << help -> botRight.y << ")" << std::endl;

                nodes.insert(pomocna);
            }
        }
    }
    return {};
}

#endif // QUADTREE_H_INCLUDED
