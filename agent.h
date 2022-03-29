#ifndef PATHPLANNINGMAPD_AGENT_H
#define PATHPLANNINGMAPD_AGENT_H

#include "coordinate.h"
#include "task.h"
#include <vector>
#include <cstdint>

class Agent {
private:
    Coordinate end_point;
    std::vector<Coordinate> path;
    int64_t start_time;
    bool is_busy;

public:
    Agent();

    Agent(Coordinate initial_position);

    void update_path(Task task);
};

#endif //PATHPLANNINGMAPD_AGENT_H
