#include "a_star.h"

AStarResult::AStarResult() : is_found(false), path_len(-1) {}

AStar::AStar() = default;

AStar::~AStar() {
    for (Node* node : to_del) {
        delete node;
    }
}

std::vector<Node*> AStar::get_neighbours(const Map& map, const Task& task, Token& token, Node* node) {
    std::vector<Node*> neighbours;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (abs(i) + abs(j) != 1) {
                continue;
            }
            // добавить опции
            int cur_i = node->i + i;
            int cur_j = node->j + j;
            if (!map.CellOnGrid(cur_i, cur_j)) {
                continue;
            }
            if (!map.CellIsTraversable(cur_i, cur_j)) {
                continue;
            }
            if (token.is_any_blocked_at_ts(node->ts_opened + 1)) { // проверяем, не занята ли данная клетка каким-то другим агентом
                const auto& blocked = token.get_locations_at_ts(node->ts_opened + 1);
                Coordinate cur_coordinate(cur_i, cur_j);
                if (blocked.find(cur_coordinate) != blocked.end()) {
                    continue;
                }
                const auto& prev_ = blocked.find({node->i, node->j});
                if (prev_ != blocked.end()) {
                    if (prev_->prev_i == cur_i && prev_->prev_j == cur_j) { // проверяем на столкновение лоб в лоб
                        continue;
                    }
                }
            }
            Node* cur_node = new Node(cur_i, cur_j);
            cur_node->g = node->g + sqrt(i * i + j * j);
            cur_node->H = token.get_precalculated_h(cur_i, cur_j, task.finish.i, task.finish.j);
            cur_node->F = cur_node->g + cur_node->H;
            cur_node->prev = node;
            cur_node->ts_opened = node->ts_opened + 1;
            neighbours.push_back(cur_node);
        }
    }
    return neighbours;
}

AStarResult AStar::find_path(const Map& map, const Task& task, Token& token, int64_t start_ts) {
    Node* start = new Node(task.start.i, task.start.j);
    start->g = 0;
    start->H = token.get_precalculated_h(start->i, start->j, task.finish.i, task.finish.j);
    start->F = start->H;
    start->ts_opened = start_ts;
    CLOSE[{start->i, start->j}] = start;
    OPEN.insert(start);
    to_del = {start};
    Node* goal = nullptr;
    while (true) {
        if (OPEN.empty()) {
            break; // провал
        }
        Node* cur_node = *OPEN.begin();
        OPEN.erase(OPEN.begin());
        visited_nodes[{cur_node->i, cur_node->j}] = nullptr;
        if (cur_node->i == task.finish.i && cur_node->j == task.finish.j) {
            goal = cur_node;
            break; // успех
        }
        std::vector<Node*> neighbours = get_neighbours(map, task, token, cur_node);
        CLOSE[{cur_node->i, cur_node->j}] = cur_node;
        for (Node* neighbour_node : neighbours) {
            to_del.push_back(neighbour_node);
            if (CLOSE.find({neighbour_node->i, neighbour_node->j}) != CLOSE.end()) {
                continue;
            }
            if (!visited_nodes[{neighbour_node->i, neighbour_node->j}]) {
                OPEN.insert(neighbour_node);
                visited_nodes[{neighbour_node->i, neighbour_node->j}] = neighbour_node;
            } else {
                Node* visited_node = visited_nodes[{neighbour_node->i, neighbour_node->j}];
                OPEN.erase(visited_node);
                if (neighbour_node->g < visited_node->g) {
                    visited_node->g = neighbour_node->g;
                    visited_node->F = neighbour_node->F;
                    visited_node->prev = cur_node;
                    visited_node->ts_opened = cur_node->ts_opened + 1;
                }
                OPEN.insert(visited_node);
            }
        }
    }
    AStarResult res;
    if (goal) {
        std::list<Node*> path;
        while (goal) {
            path.push_front(goal);
            goal = goal->prev;
        }
        res.is_found = true;
        res.path_len = path.size() - 1;
        res.path = path;
        return res;
    } else {
        res.is_found = false;
        return res;
    }
}

