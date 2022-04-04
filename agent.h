#ifndef PATHPLANNINGMAPD_AGENT_H
#define PATHPLANNINGMAPD_AGENT_H

#include "coordinate.h"
#include "task.h"
#include <vector>
#include <cstdint>
#include "a_star.h"
#include "map.h"

class Agent {
private:
    Coordinate start_coordinate;
    std::vector<Coordinate> path_to_start;
    std::vector<Coordinate> path_to_finish;
    int64_t start_time;
    bool is_busy;

public:
    Agent();

    Agent(Coordinate initial_position);

    bool update_path(const Map& map, const Task& task, Token& token);
};

#endif //PATHPLANNINGMAPD_AGENT_H
