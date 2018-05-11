#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "Point.h"
#include "Position.h"

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

#endif // NODE_H_INCLUDED
