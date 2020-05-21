//
// Created by dbojadzic on 13.05.20.
//

#ifndef ZAVRSNI_POSE_H
#define ZAVRSNI_POSE_H

struct Pose {
    double x{};
    double y{};
    double th{};
    Pose *previous{};

    Pose() = default;
    Pose(double x, double y, double th, Pose *previous = nullptr) : x(x), y(y), th(th), previous(previous) {};

    bool operator==(Pose pose) const {
        return x == pose.x && y == pose.y && th == pose.th;
    }
};


#endif //ZAVRSNI_POSE_H
