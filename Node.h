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

    void Unite();
    //void SearchChildren(std::vector<Node*> &neighbors, Position a, Position b);
    void FindBordering(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void FindBorderingNoOccupied(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void SetHeuristics(Node* referent);
    bool SetWeight(Node* node);
    void Write();
    void WriteAll();
    std::string WriteMatlab();
};

void Node::Unite(){

    //if(topLeftTree)
    //    std::cout << "call:" << topLeftTree->occupied << topRightTree->occupied << botLeftTree->occupied << botRightTree->occupied << depth << std::endl;
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
/*
void Node::SearchChildren(std::vector<Node*> &neighbors, Position a, Position b){
    if(!topLeftTree){
        neighbors.push_back(this);
        return;
    }
    if(topLeftTree -> position == a || topLeftTree -> position == b){
            topLeftTree -> SearchChildren(neighbors, a, b);
    }
    if(topRightTree -> position == a || topRightTree -> position == b){
            topRightTree -> SearchChildren(neighbors, a, b);
    }
    if(botLeftTree -> position == a || botLeftTree -> position == b){
            botLeftTree -> SearchChildren(neighbors, a, b);
    }
    if(botRightTree -> position == a || botRightTree -> position == b){
            botRightTree -> SearchChildren(neighbors, a, b);
    }
}
*/

void Node::FindBordering(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight){
    if(topLeft == _topLeft && botRight == _botRight)
        return;

    if(!topLeftTree){
        neighbors.push_back(this);
        return;
    }

    Point topLeftCopy{topLeftTree -> topLeft};
    Point botRightCopy{topLeftTree -> botRight};

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
        topLeftTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    topLeftCopy = topRightTree -> topLeft;
    botRightCopy = topRightTree -> botRight;

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
        topRightTree -> FindBordering(neighbors, _topLeft, _botRight);
    }
    topLeftCopy = botLeftTree -> topLeft;
    botRightCopy = botLeftTree -> botRight;

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
        botLeftTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    topLeftCopy = botRightTree -> topLeft;
    botRightCopy = botRightTree -> botRight;

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
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

    Point topLeftCopy{topLeftTree -> topLeft};
    Point botRightCopy{topLeftTree -> botRight};

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
        topLeftTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    topLeftCopy = topRightTree -> topLeft;
    botRightCopy = topRightTree -> botRight;

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
        topRightTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }
    topLeftCopy = botLeftTree -> topLeft;
    botRightCopy = botLeftTree -> botRight;

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
        botLeftTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    topLeftCopy = botRightTree -> topLeft;
    botRightCopy = botRightTree -> botRight;

    if(topLeftCopy.x <= _topLeft.x && _topLeft.x <= botRightCopy.x && (topLeftCopy.y <= _topLeft.y && _topLeft.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y) || topLeftCopy.x <= _botRight.x && _botRight.x <= botRightCopy.x && (topLeftCopy.y <= _botRight.y && _botRight.y <= botRightCopy.y || _topLeft.y <= topLeftCopy.y && botRightCopy.y <= _botRight.y)){
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
        std::cout << std::endl << "(" << topLeft.x << "," << topLeft.y << "),(" << botRight.x << "," << botRight.y << ")" << " Weight: " << weight;
        if(cameFrom)
            std::cout << ", Came From: " << "(" << cameFrom -> topLeft.x << "," << cameFrom -> topLeft.y << "),(" << cameFrom -> botRight.x << "," << cameFrom -> botRight.y << ")" << std::endl;
         else
            std::cout << ", Came From Nothing" << std::endl;
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

#endif // NODE_H_INCLUDED
