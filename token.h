#ifndef PATHPLANNINGMAPD_TOKEN_H
#define PATHPLANNINGMAPD_TOKEN_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <Task>

class Token {
    std::unordered_map<int64_t, std::unordered_set<Coordinate>> blocked_cells_per_ts; // for each timestep set of blocked coordinates (where agents locate)
    std::unordered_set<Task> tasks;
};

#endif //PATHPLANNINGMAPD_TOKEN_H
