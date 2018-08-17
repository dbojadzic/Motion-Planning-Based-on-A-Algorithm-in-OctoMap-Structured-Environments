#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

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

    double GetWidth() const;
    double GetHeight() const;
    void InsertPoint(const Point &point);
    bool IsOccupied(Point point);
    Node* FindNode(Point point);
    int CountNodes();
    std::vector<Node*> FindAdjacent(Node* node);
    std::vector<Node*> FindAdjacentNoOccupied(Node* node);
    void WriteAll();
    void WriteMapMatlab(std::string path);
    void StreamMapMatlab(std::stringstream &izvjestaj);
    void Reset();
};

QuadTree::QuadTree(int matrix[], int width, int height) : topLeft(0, 0), botRight(width, height){
    root = new Node(nullptr, topLeft, botRight, 0);
    for(int i{}; i<width; i++){
        for (int j{}; j<height; j++){
            if(!matrix[j*height + i]){
                //std::cout << "B: " << i << "," << j << std::endl;
                InsertPoint(Point(i,j));
            }
        }
    }
}

double QuadTree::GetWidth() const {
    return botRight.x - topLeft.x;
}

double QuadTree::GetHeight() const {
    return botRight.y - topLeft.y;
}

void QuadTree::InsertPoint(const Point &point){
    //std::cout << point.x << "," << point.y << std::endl;
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
    //std::cout << point.x << "," << point.y << std::endl;
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

int QuadTree::CountNodes(){
    return root -> Count();
}

std::vector<Node*> QuadTree::FindAdjacentNoOccupied(Node* node){
    std::vector<Node*> adjecent;
    root -> FindBorderingNoOccupied(adjecent, node -> topLeft, node -> botRight);
    return adjecent;
}

void QuadTree::WriteAll(){
    root -> WriteAll();
}

void QuadTree::WriteMapMatlab(std::string path){
    std::ofstream izvjestaj(path);
    std::queue<Node*> red;
    red.push(root);
    while(!red.empty()){
        Node* help = red.front();
        red.pop();
        if(help -> topLeftTree){
            red.push(help -> topLeftTree);
            red.push(help -> topRightTree);
            red.push(help -> botLeftTree);
            red.push(help -> botRightTree);
        }
        else {
            izvjestaj << "rectangle('Position',[" << help -> WriteMatlab() << "], 'FaceColor',[";
            if(help -> occupied){
                izvjestaj << "0 0 0])" << std::endl;
            }
            else {
                izvjestaj << "1 1 1])" << std::endl;
            }
        }
    }
}

void QuadTree::StreamMapMatlab(std::stringstream &izvjestaj){
    std::queue<Node*> red;
    red.push(root);
    while(!red.empty()){
        Node* help = red.front();
        red.pop();
        if(help -> topLeftTree){
            red.push(help -> topLeftTree);
            red.push(help -> topRightTree);
            red.push(help -> botLeftTree);
            red.push(help -> botRightTree);
        }
        else {
            izvjestaj << "rectangle('Position',[" << help -> WriteMatlab() << "], 'FaceColor',[";
            if(help -> occupied){
                izvjestaj << "0 0 0]);" << std::endl;
            }
            else {
                izvjestaj << "1 1 1]);" << std::endl;
            }
        }
    }
}

void QuadTree::Reset() {
    root -> Reset();
}


#endif // QUADTREE_H_INCLUDED
