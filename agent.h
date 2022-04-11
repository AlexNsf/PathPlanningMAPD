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
    int64_t finish_time;
    int num;

public:
    Agent();

    Agent(Coordinate initial_position);

    bool update_path(const Map& map, const Task& task, Token& token);

    int64_t get_finish_time() const;
};

#endif //PATHPLANNINGMAPD_AGENT_H
