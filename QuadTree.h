#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

#include "Point.h"
#include "Node.h"
#include "Position.h"

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

    /*
    std::vector<Node*> adjecent;
    if(!node -> parent){
        return adjecent;
    }
    std::vector<Position> positions;
    Node* current{node};
    while(current -> parent){
        positions.push_back(current -> position);
        current = current -> parent;
    }
    current = node;
    bool top{}, bot{}, left{}, right{};
    for(int i{}; i<positions.size(); i++){
        current = current -> parent;
        std::queue<Node*> nodes;
        if(current -> topLeftTree -> position != positions[i]){
            current -> topLeftTree -> SearchChildren(adjecent, positions[i], !current -> topLeftTree -> position);
        }
        if(current -> topRightTree -> position != positions[i]){
            current -> topRightTree -> SearchChildren(adjecent, positions[i], !current -> topRightTree -> position);
        }
        if(current -> botLeftTree -> position != positions[i]){
            current -> botLeftTree -> SearchChildren(adjecent, positions[i], !current -> botLeftTree -> position);
        }
        if(current -> botRightTree -> position != positions[i]){
            current -> botRightTree -> SearchChildren(adjecent, positions[i], !current -> botRightTree -> position);
        }

    }
    return adjecent;
    */
}

#endif // QUADTREE_H_INCLUDED
