#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "Point.h"

struct Node{
    Point topLeft;
    Point botRight;
    bool occupied = false;
    int depth;

    Node* parent = nullptr;
    Node* topLeftTree = nullptr;
    Node* topRightTree = nullptr;
    Node* botLeftTree = nullptr;
    Node* botRightTree = nullptr;

    double fScore = std::numeric_limits<double>::infinity();
    double gScore = std::numeric_limits<double>::infinity();
    Node* cameFrom = nullptr;

    int borderPoints = 0;
    Point AccessPoint;
    Point BestPoint;

    Node(Node* parent, Point topLeft, Point botRight, int depth,  int points = 0, bool occupied = false) : parent(parent), topLeft(topLeft), botRight(botRight), occupied(occupied), depth(depth), borderPoints(points){}

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

    Point GetCenter() const ;
    void Unite();
    bool Overlap(const Point &_topLeft, const Point &_botRight) const;
    void FindBordering(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void FindBorderingNoOccupied(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void Write() const;
    void WriteAll() const;
    std::string WriteMatlab() const;
    void Reset();
    std::vector<Point> GetBorderPoints() const;
    std::vector<Point> GetBorder() const;
};

Point Node::GetCenter() const {
    return {(topLeft.x + botRight.x)/2, (topLeft.y + botRight.y)/2};
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

bool Node::Overlap(const Point &_topLeft, const Point &_botRight) const {
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

void Node::Write() const {
        std::cout << " (" << topLeft.x << "," << topLeft.y << "),(" << botRight.x << "," << botRight.y << ")" << " fScore: " << fScore << ", gScore: " << gScore;
        if(cameFrom)
            std::cout << ", Came From: " << "(" << cameFrom -> topLeft.x << "," << cameFrom -> topLeft.y << "),(" << cameFrom -> botRight.x << "," << cameFrom -> botRight.y << ")";
         else
            std::cout << ", Came From Nothing";
}

void Node::WriteAll() const {
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

std::string Node::WriteMatlab() const {
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

void Node::Reset() {
    fScore = std::numeric_limits<double>::infinity();
    gScore = std::numeric_limits<double>::infinity();
    cameFrom = nullptr;
    if(!topLeftTree)
        return;
    topLeftTree -> Reset();
    topRightTree -> Reset();
    botLeftTree -> Reset();
    botRightTree -> Reset();
}

std::vector<Point> Node::GetBorderPoints() const {
    std::vector<Point> p;
    double constx1, constx2, consty1, consty2;
    long double delta;
    constx1 = topLeft.x;
    consty1 = topLeft.y;
    consty2 = botRight.y;
    delta = (topLeft.x - botRight.x)/(borderPoints+1);
    for(int i{}; i<borderPoints+2; i++){
        p.emplace_back(constx1 + i*delta, consty1);
        p.emplace_back(constx1 + i*delta, consty2);
    }
    constx2 = botRight.x;
    delta = (botRight.y - topLeft.y)/(borderPoints+1);
    for(int i{}; i<borderPoints+2; i++){
        p.emplace_back(constx1, consty1 + i*delta);
        p.emplace_back(constx2, consty1 + i*delta);
    }
    return p;
}

std::vector<Point> Node::GetBorder() const {
    std::vector<Point> b;
    double constv1, constv2;
    long double delta;
    if(botRight.x == cameFrom -> topLeft.x){
        constv1 = topLeft.x;
        constv2 = topLeft.y < botRight.y ? topLeft.y : botRight.y;
        delta = (fabs(topLeft.x - botRight.x))/(borderPoints+1);
        for(int i{}; i<borderPoints+2; i++){
            b.emplace_back(constv1, constv2 + i*delta);
        }
    }
    else if (topLeft.x == cameFrom -> botRight.x){
        constv1 = topLeft.x;
        constv2 = topLeft.y < botRight.y ? topLeft.y : botRight.y;
        delta = (fabs(topLeft.x - botRight.x))/(borderPoints+1);
        for(int i{}; i<borderPoints+2; i++){
            b.emplace_back(constv1, constv2 + i*delta);
        }
    }
    else if (topLeft.y == cameFrom -> botRight.y){
        constv1 = topLeft.y;
        constv2 = topLeft.x < botRight.x ? topLeft.x : botRight.x;
        delta = (fabs(topLeft.y - botRight.y))/(borderPoints+1);
        for(int i{}; i<borderPoints+2; i++){
            b.emplace_back(constv1 + i*delta, constv2);
        }
    }
    else if ( botRight.y == cameFrom -> topLeft.y){
            constv1 = topLeft.y;
            constv2 = topLeft.x < botRight.x ? topLeft.x : botRight.x;
            delta = (fabs(topLeft.y - botRight.y))/(borderPoints+1);
            for(int i{}; i<borderPoints+2; i++){
                b.emplace_back(constv1 + i*delta, constv2);
            }
    }
    else {
        throw "Fatal error, no border";
    }
    return b;
}

double Distance(const Node* a, const Node* b) {
    return sqrt(pow(b -> GetCenter().x - a -> GetCenter().x, 2) + pow(b -> GetCenter().y - a -> GetCenter().y, 2));
}

#endif // NODE_H_INCLUDED
