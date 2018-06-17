#ifndef ASTARMATRIX_H_INCLUDED
#define ASTARMATRIX_H_INCLUDED

#include "map"
#include "Point.h"

double heuristic_cost_estimate(Point start, Point goal){
    std::cout << "Loo";
    return sqrt(pow(goal.x - start.x, 2) + pow(goal.y - start.y, 2));
}

double dist_between(Point start, Point goal){
    return sqrt(pow(goal.x - start.x, 2) + pow(goal.y - start.y, 2));
}

std::vector<Point> reconstruct_path(std::map<Point, Point, Point> cameFrom, Point current){
    std::vector<Point> total_path{current};
    while(cameFrom.count(current)){
        current = cameFrom[current];
        total_path.push_back(current);
    }
    return total_path;
}

class AstarMatrix{
private:

    int* matrix;
    int width;
    int height;

public:

    AstarMatrix(int* matrix, int width, int height) : matrix(matrix), width(width), height(height) {}

    std::vector<Point> FindPath(Point start, Point finish);
    double FindDistance(Point start, Point finish);
    double FindDistance(std::vector<Point> nodes, Point start, Point finish);
    void CreateFullMatlabPlot(std::string path, Point start, Point finish);

};

std::vector<Point> AstarMatrix::FindPath(Point start, Point goal){
    if(!matrix[(int)(start.y*height + start.x)])
        return {};
     if(!matrix[(int)(goal.y*height + goal.x)])
        return {};
    std::set<Point, Point> closedSet;
    std::set<Point, Point> openSet;
    openSet.insert(start);
    std::map<Point, Point, Point> cameFrom;
    std::map<Point, double, Point> gScore;
    gScore[start] = 0;
    std::map<Point, double, Point> fScore;
    fScore[start] = heuristic_cost_estimate(start, goal);
    while(!openSet.empty()){
        Point current;
        double minimum{std::numeric_limits<double>::infinity()};
        for(Point point : openSet){
            if(fScore.count(point)){
                if(minimum > fScore[point]){
                    minimum = fScore[point];
                    current = point;
                }
            }
        }
        if(current == goal)
            return reconstruct_path(cameFrom, current);
        openSet.erase(current);
        closedSet.insert(current);
        for(int i{current.x - 1}; i <= current.x + 1; i++){
            if(i < 0 || i >= width)
                continue;
            for(int j{current.y - 1}; j <= current.y + 1 ; j++){
                if(j < 0 || j >= height)
                    continue;
                if(i==current.x && j==current.y)
                    continue;
                if(!matrix[j*height + i])
                    continue;

                Point neighbor(i,j);
                std::cout << neighbor.x << "," << neighbor.y << std::endl;
                if(closedSet.count(neighbor))
                    continue;
                openSet.insert(neighbor);
                double tentative_gScore{};
                if(!gScore.count(current))
                    gScore[current] = std::numeric_limits<double>::infinity();
                if(!gScore.count(neighbor))
                    gScore[neighbor] = std::numeric_limits<double>::infinity();
                std::cout << "A";
                tentative_gScore = gScore[current] + dist_between(current, neighbor);
                if(tentative_gScore >= gScore[neighbor])
                    continue;
                std::cout << "B";
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                std::cout << "fuu" << std::endl;
                std::cout << neighbor.x << "," << neighbor.y << std::endl;
                std::cout << tentative_gScore << std::endl;
                std::cout << heuristic_cost_estimate(neighbor, goal) << std::endl;
                std::cout << fScore[neighbor] << std::endl;
                fScore[neighbor] = tentative_gScore + heuristic_cost_estimate(neighbor, goal);
                std::cout << "luu";
            }
        }
    }
    return {};
}

void AstarMatrix::CreateFullMatlabPlot(std::string path, Point start, Point finish){
    std::stringstream instructions;
    instructions << "axis([0 " << width << " " << -1 * height << " 0]);" << std::endl;
    instructions << "title('Matrix: " << width << " x " << height << " From: (" << start.x << "," << start.y << ") To: (" << finish.x << "," << finish.y << ")');" << std::endl;
    instructions << "xlabel('Width: " << width << "');" << std::endl;
    instructions << "ylabel('Height: " << height << "');" << std::endl;
    /*
    for(int i{}; i<width; i++){
        for(int j{}; j<height; j++){
            instructions << "rectangle('Position',[" << i - 0.5 << " " << -j - 0.5 << " 1 1], 'FaceColor',[";
            if(matrix[i*height + j]){
                instructions << "0 0 0]);" << std::endl;
            }
            else {
                instructions << "1 1 1]);" << std::endl;
            }
        }
    }
    */
    std::vector<Point> points{FindPath(start, finish)};
    instructions << "X=[";
    instructions << finish.x << ",";
    for(Point point : points){
        instructions << point.x << ",";
    }
    instructions << start.x;
    instructions << "];" << std::endl;
    instructions << "Y=[";
    instructions << -finish.y << ",";
    for(Point point : points){
        instructions << point.y * -1 << ",";
    }
    instructions << -start.y;
    instructions << "];" << std::endl;
    instructions << "hold on;" << std::endl;
    instructions << "plot(X,Y,'r');" << std::endl;
    //instructions << "legend('" << FindDistance(nodes, start, finish) << "','Location','southoutside');" << std::endl;
    std::ofstream write(path);
    write << instructions.str();

}








#endif // ASTARMATRIX_H_INCLUDED
