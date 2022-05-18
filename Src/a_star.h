#ifndef PATHPLANNINGMAPD_A_STAR_H
#define PATHPLANNINGMAPD_A_STAR_H

#include "token.h"
#include "task.h"
#include "map.h"
#include "node.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <list>

struct AStarResult {
    bool is_found;
    size_t path_len;
    std::list<Node*> path;

    AStarResult();
};

class AStar {
private:
    std::set<Node*, NodeComparator> OPEN;
    std::map<std::pair<int, int>, Node*> CLOSE;
    std::map<std::pair<int, int>, Node*> visited_nodes;
    std::vector<Node*> to_del;
    // попробовать поменять map на unordered_map

public:
    AStar();

    ~AStar();

    AStarResult find_path(const Map& map, const Task& task, Token& token, int64_t start_ts);

    std::vector<Node*> get_neighbours(const Map& map, const Task& task, Token& token, Node* node);
};

#endif //PATHPLANNINGMAPD_A_STAR_H
