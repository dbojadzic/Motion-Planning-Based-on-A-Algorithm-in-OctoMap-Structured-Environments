//
// Created by dbojadzic on 13.05.20.
//

#ifndef ZAVRSNI_RRT_H
#define ZAVRSNI_RRT_H

#include <algorithm>
#include <cmath>
#include "Pose.h"

class RRT{
private:
    QuadTree* quadtree{};
    double integration_step{0.01};
    double step_size{0.1};
    double velocity{3};
    double length{3};
    int steering_samples{20};
public:
    RRT(QuadTree* quadtree) : quadtree(quadtree) {};
    std::vector<Pose> RefinePath(std::vector<Node*> &nodes, Pose start, Pose finish);
};

std::vector<Pose> RRT::RefinePath(std::vector<Node*> &nodes, Pose start, Pose finish){
    std::vector<Pose *> tree;
    tree.push_back(new Pose(start.x, start.y, start.th, nullptr));
    int sample{static_cast<int>(nodes.size()-2)};
    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

    bool goal_reached{false};

    while(!goal_reached){
        std::uniform_real_distribution<double> coin(0, 1);
        Point p;
        if(coin(generator) < 0.75) {
            p = nodes[sample]->RandomPoint();
        } else {
            p = nodes[sample+1]->RandomPoint();
        }
        std::cout << sample << ": " << p.x << "," << p.y << std::endl;
        Pose *closest{*std::min_element(tree.begin(), tree.end(), [p](Pose *a, Pose *b){
            return sqrt(pow(a->x - p.x, 2) + pow(a->y - p.y, 2)) < sqrt(pow(b->x - p.x, 2) + pow(b->y - p.y, 2));
        })};
        std::vector<Pose *>new_poses;
        for(int i{}; i <= steering_samples; i++){
            double delta{-atan(1) + double(i)/steering_samples * 2 * atan(1)};
            //std::cout << delta << std::endl;
            Pose *new_pose{new Pose(*closest)};
            for (int j{}; j < step_size / integration_step; j++) {
                new_pose->x += velocity * cos(new_pose->th) * integration_step;
                new_pose->y += velocity * sin(new_pose->th) * integration_step;
                new_pose->th += velocity/3*tan(delta)*integration_step;
            }
            new_pose->previous = closest;
            try {
                if (quadtree->IsOccupied(*new_pose)) {
                    continue;
                }
            } catch (...){
                continue;
            }
            new_poses.push_back(new_pose);
        }
        if(!new_poses.empty()) {
            Pose *new_closest{*std::min_element(new_poses.begin(), new_poses.end(), [p](Pose *a, Pose *b){
                return sqrt(pow(a->x - p.x, 2) + pow(a->y - p.y, 2)) < sqrt(pow(b->x - p.x, 2) + pow(b->y - p.y, 2));
            })};
            tree.push_back(new_closest);
            std::cout << "[" << new_closest->x << "," << new_closest->y << "]" << std::endl;
            if (quadtree->FindNode(*new_closest) == (nodes[sample])) {
                sample--;
                std::cout << "Next sample" << std::endl;
            }
            if (sample < 0) {
                goal_reached = true;
                std::vector<Pose> trajectory;
                Pose *pose{new_closest};
                while (pose) {
                    trajectory.push_back(*pose);
                    pose = pose->previous;
                }
                for(Pose* pos : tree){
                    delete pos;
                }
                return trajectory;
            }
            for(Pose *pos : new_poses){
                if(pos != new_closest)
                    delete pos;
            }
        }
    }
}

#endif //ZAVRSNI_RRT_H
