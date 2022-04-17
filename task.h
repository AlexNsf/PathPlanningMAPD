#ifndef PATHPLANNINGMAPD_TASK_H
#define PATHPLANNINGMAPD_TASK_H

#include "coordinate.h"

struct Task {
    Coordinate start;
    Coordinate finish;
    int num;

    Task() : start(-1, -1), finish(-1, -1) {
        static int task_counter = 0;
        num = ++task_counter;
    }

    Task(Coordinate start_, Coordinate finish_) : start(start_), finish(finish_) {
        static int task_counter = 0;
        num = ++task_counter;
    }
};

inline bool operator==(const Task& t_1, const Task& t_2) {
    return t_1.start == t_2.start && t_1.finish == t_2.finish && t_1.num == t_2.num;
}

inline bool operator!=(const Task& t_1, const Task& t_2) {
    return !(t_1 == t_2);
}

struct TaskComparator {
    bool operator()(const Task& t_1, const Task& t_2) const {
        return std::tie(t_1.start, t_1.finish, t_1.num) < std::tie(t_2.start, t_2.finish, t_2.num);
    }
};

#endif //PATHPLANNINGMAPD_TASK_H
