#ifndef PATHPLANNINGMAPD_TASK_H
#define PATHPLANNINGMAPD_TASK_H

#include "coordinate.h"

struct Task {
    Coordinate start;
    Coordinate finish;

    Task() : start(-1, -1), finish(-1, -1) {}

    Task(Coordinate start_, Coordinate finish_) : start(start_), finish(finish_) {}
};

#endif //PATHPLANNINGMAPD_TASK_H
