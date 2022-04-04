#include "agent.h"

Agent::Agent() {
    start_time = -1;
    is_busy = false;
}

Agent::Agent(Coordinate initial_position) {
    start_coordinate = initial_position;
    start_time = 0;
    is_busy = false;
}

bool Agent::update_path(const Map& map, const Task& task, Token& token) {
    is_busy = true;
    start_time = token.get_cur_ts();
    path_to_start.clear();
    AStar search_to_start;
    Task task_to_start(start_coordinate, task.start);
    auto search_res_to_start = search_to_start.find_path(map, task_to_start, token, start_time);
    if (!search_res_to_start.is_found) {
        token.add_blocked_cell_at_ts(start_coordinate, start_time + 1);
        return false;
    }
    for (auto& node : search_res_to_start.path) {
        Coordinate cur_coordinate(node->i, node->j);
        path_to_start.push_back(cur_coordinate);
    }
    path_to_finish.clear();
    AStar search_to_finish;
    auto search_res_to_finish = search_to_finish.find_path(map, task, token, start_time + search_res_to_start.path_len);
    if (!search_res_to_finish.is_found) {
        token.add_blocked_cell_at_ts(start_coordinate, start_time + 1);
        return false;
    }
    for (int i = 0; i < path_to_start.size(); ++i) {
        token.add_blocked_cell_at_ts(path_to_start[i], start_time + i);
    }
    for (int i = 0; i < path_to_finish.size(); ++i) {
        token.add_blocked_cell_at_ts(path_to_finish[i], start_time + search_res_to_start.path_len + i);
    }
    start_coordinate = task.finish;
    return true;
}
