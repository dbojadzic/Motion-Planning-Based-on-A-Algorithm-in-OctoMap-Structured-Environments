#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "Point.h"
#include "Position.h"

struct Node{
    Point topLeft;
    Point botRight;
    bool occupied = false;
    int depth;

    double heuristics = std::numeric_limits<double>::infinity();
    double weight = std::numeric_limits<double>::infinity();
    Node* cameFrom = nullptr;
    Node* maybeCameFrom = nullptr;

    Node* parent = nullptr;
    Node* topLeftTree = nullptr;
    Node* topRightTree = nullptr;
    Node* botLeftTree = nullptr;
    Node* botRightTree = nullptr;

    Node(Node* parent, Point topLeft, Point botRight, int depth, bool occupied = false) : parent(parent), topLeft(topLeft), botRight(botRight), occupied(occupied), depth(depth){}

    ~Node(){
        delete topLeftTree;
        delete topRightTree;
        delete botLeftTree;
        delete botRightTree;
    }

    int Count(){
        if(topLeftTree)
            return topLeftTree -> Count() + topRightTree -> Count() + botLeftTree -> Count() + botRightTree -> Count() + 1;
        else
            return 1;
    }

    Point GetCenter();
    void Unite();
    bool Overlap(const Point &_topLeft, const Point &_botRight);
    void FindBordering(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void FindBorderingNoOccupied(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void SetHeuristics(Node* referent);
    bool SetWeight(Node* node);
    void Write();
    void WriteAll();
    std::string WriteMatlab();
};

Point Node::GetCenter(){
    return Point((topLeft.x + botRight.x)/2, (topLeft.y + botRight.y)/2);
}

void Node::Unite(){
    if(topLeftTree && topLeftTree->occupied && topRightTree->occupied && botLeftTree->occupied && botRightTree->occupied){
        delete topLeftTree;
        delete topRightTree;
        delete botLeftTree;
        delete botRightTree;
        topLeftTree = nullptr;
        topRightTree = nullptr;
        botLeftTree = nullptr;
        botRightTree = nullptr;
        occupied = true;
        if(parent)
            parent->Unite();
    }

}

bool Node::Overlap(const Point &_topLeft, const Point &_botRight){
    Point topLeftCopy{topLeft};
    Point botRightCopy{botRight};
    return (topLeftCopy.x <= _topLeft.x
            && _topLeft.x <= botRightCopy.x
            && (topLeftCopy.y <= _topLeft.y
                && _topLeft.y <= botRightCopy.y
                || topLeftCopy.y <= _botRight.y
                && _botRight.y <= botRightCopy.y
                || _topLeft.y <= topLeftCopy.y
                && botRightCopy.y <= _botRight.y)
            || topLeftCopy.x <= _botRight.x
            && _botRight.x <= botRightCopy.x
            && (topLeftCopy.y <= _botRight.y
                && _botRight.y <= botRightCopy.y
                || topLeftCopy.y <= _topLeft.y
                && _topLeft.y <= botRightCopy.y
                || _topLeft.y <= topLeftCopy.y
                && botRightCopy.y <= _botRight.y)
            || _topLeft.x <= topLeftCopy.x
            && botRightCopy.x <= _botRight.x
            && (_topLeft.y == botRightCopy.y
                || _botRight.y == topLeftCopy.y));
}

void Node::FindBordering(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight){
    if(topLeft == _topLeft && botRight == _botRight)
        return;

    if(!topLeftTree){
        neighbors.push_back(this);
        return;
    }

    if(topLeftTree -> Overlap(_topLeft, _botRight)){
        topLeftTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    if(topRightTree -> Overlap(_topLeft, _botRight)){
        topRightTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    if(botLeftTree -> Overlap(_topLeft, _botRight)){
        botLeftTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    if(botRightTree -> Overlap(_topLeft, _botRight)){
        botRightTree -> FindBordering(neighbors, _topLeft, _botRight);
    }
}

void Node::FindBorderingNoOccupied(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight){
    if(topLeft == _topLeft && botRight == _botRight)
        return;

    if(occupied)
        return;

    if(!topLeftTree){
        neighbors.push_back(this);
        return;
    }

    if(topLeftTree -> Overlap(_topLeft, _botRight)){
        topLeftTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    if(topRightTree -> Overlap(_topLeft, _botRight)){
        topRightTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    if(botLeftTree -> Overlap(_topLeft, _botRight)){
        botLeftTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    if(botRightTree -> Overlap(_topLeft, _botRight)){
        botRightTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }
}

void Node::SetHeuristics(Node* referent){
    cameFrom = nullptr;
    weight = std::numeric_limits<double>::infinity();
    heuristics = std::numeric_limits<double>::infinity();

    if(topLeftTree){
        topLeftTree -> SetHeuristics(referent);
        topRightTree -> SetHeuristics(referent);
        botLeftTree -> SetHeuristics(referent);
        botRightTree -> SetHeuristics(referent);
    }
    else{
        double referentx = (referent -> botRight.x + referent -> topLeft.x)/2;
        double referenty = (referent -> botRight.y + referent -> topLeft.y)/2;
        double currentx = (botRight.x + topLeft.x)/2;
        double currenty = (botRight.y + topLeft.y)/2;
        heuristics = sqrt(pow(referentx - currentx, 2) + pow(referenty - currenty, 2));
    }
}

bool Node::SetWeight(Node* node){
    double referentx = (node -> botRight.x + node -> topLeft.x)/2;
    double referenty = (node -> botRight.y + node -> topLeft.y)/2;
    double currentx = (botRight.x + topLeft.x)/2;
    double currenty = (botRight.y + topLeft.y)/2;
    double newweight = heuristics + sqrt(pow(referentx - currentx, 2) + pow(referenty - currenty, 2));
    if(weight > newweight){
        weight = newweight;
        maybeCameFrom = node;
        return true;
    }
    return false;
}

void Node::Write(){
        std::cout << " (" << topLeft.x << "," << topLeft.y << "),(" << botRight.x << "," << botRight.y << ")" << " Weight: " << weight << ", Heuristics: " << heuristics;
        if(cameFrom)
            std::cout << ", Came From: " << "(" << cameFrom -> topLeft.x << "," << cameFrom -> topLeft.y << "),(" << cameFrom -> botRight.x << "," << cameFrom -> botRight.y << ")";
         else
            std::cout << ", Came From Nothing";
}

void Node::WriteAll(){
    if(!topLeftTree){
        Write();
    }
    else{
        topLeftTree -> WriteAll();
        topRightTree -> WriteAll();
        botLeftTree -> WriteAll();
        botRightTree -> WriteAll();
    }
}

std::string Node::WriteMatlab(){
    double x = topLeft.x;
    double y = -botRight.y;
    double width = botRight.x - x;
    std::stringstream help;
    help << x;
    help << " ";
    help << y;
    help << " ";
    help << width;
    help << " ";
    help << width;
    return help.str();
}


double Distance(Node* a, Node* b){
    return sqrt(pow(b -> GetCenter().x - a -> GetCenter().x, 2) + pow(b -> GetCenter().y - a -> GetCenter().y, 2));
}

#endif // NODE_H_INCLUDED
