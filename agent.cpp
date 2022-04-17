#include "agent.h"

Agent::Agent() {
    start_time = -1;
    finish_time = -1;
    static int agent_counter = 0;
    num = ++agent_counter;
}

Agent::Agent(Coordinate initial_position) {
    start_coordinate = initial_position;
    start_time = 0;
    finish_time = 0;
    static int agent_counter = 0;
    num = ++agent_counter;
}

bool Agent::update_path(const Map& map, const Task& task, Token& token) {
    std::cout << start_coordinate.i << ' ' << start_coordinate.j << " START\n";
    start_time = token.get_cur_ts();
    path_to_start.clear();
    AStar search_to_start;
    Task task_to_start(start_coordinate, task.start);
    std::cout << task_to_start.start.i << ' ' << task_to_start.start.j << ' ' << task_to_start.finish.i << ' ' << task_to_start.finish.j << '\n';
    auto search_res_to_start = search_to_start.find_path(map, task_to_start, token, start_time);
    if (!search_res_to_start.is_found) {
//        std::cout << "ASDSADAKSJGDKASHDGKJASD\n";
        start_coordinate.prev_i = start_coordinate.i;
        start_coordinate.prev_j = start_coordinate.j;
        start_coordinate.num = this->num;
        token.add_blocked_cell_at_ts(start_coordinate, start_time);
        token.add_blocked_cell_at_ts(start_coordinate, start_time + 1);
        finish_time = start_time + 1;
        return false;
    }

    std::cout << "PATH TO START\n";
    for (auto& node : search_res_to_start.path) {
        Coordinate cur_coordinate(node->i, node->j);
        std::cout << cur_coordinate.i << ' ' << cur_coordinate.j << '\n';
        path_to_start.push_back(cur_coordinate);
    }
    path_to_finish.clear();
    AStar search_to_finish;
    auto search_res_to_finish = search_to_finish.find_path(map, task, token, start_time + search_res_to_start.path_len);
    if (!search_res_to_finish.is_found) {
        start_coordinate.prev_i = start_coordinate.i;
        start_coordinate.prev_j = start_coordinate.j;
        start_coordinate.num = this->num;
        token.add_blocked_cell_at_ts(start_coordinate, start_time);
        token.add_blocked_cell_at_ts(start_coordinate, start_time + 1);
        finish_time = start_time + 1;
        return false;
    }
    std::cout << "PATH TO FINISH\n";
    for (auto& node : search_res_to_finish.path) {
        Coordinate cur_coordinate(node->i, node->j);
        std::cout << cur_coordinate.i << ' ' << cur_coordinate.j << '\n';
        path_to_finish.push_back(cur_coordinate);
    }
    for (int i = 0; i < path_to_start.size() - 1; ++i) {
        if (i == 0) {
            path_to_start[i].prev_i = start_coordinate.i;
            path_to_start[i].prev_j = start_coordinate.j;
        } else {
            path_to_start[i].prev_i = path_to_start[i - 1].i;
            path_to_start[i].prev_j = path_to_start[i - 1].j;
        }
        path_to_start[i].num = this->num;
        token.add_blocked_cell_at_ts(path_to_start[i], start_time + i);
        token.add_blocked_endpoint(start_coordinate, start_time + i); // занятый финиш
    }
    for (int i = 0; i < path_to_finish.size(); ++i) {
        if (i == 0) {
            path_to_finish[i].prev_i = path_to_start[path_to_start.size() - 2].i;
            path_to_finish[i].prev_j = path_to_start[path_to_start.size() - 2].j;
            path_to_finish[i].started = true;
//            std::cout << "ASDSDADADAd\n";
        } else {
            path_to_finish[i].prev_i = path_to_finish[i - 1].i;
            path_to_finish[i].prev_j = path_to_finish[i - 1].j;
        }
        if (i == path_to_finish.size() - 1) {
            path_to_finish[i].finished = true;
        }
        path_to_finish[i].num = this->num;
        token.add_blocked_cell_at_ts(path_to_finish[i], start_time + search_res_to_start.path_len + i);
        token.add_blocked_endpoint(task.finish, start_time + search_res_to_start.path_len + i); // занятый финиш
    }
    start_coordinate = task.finish;
    finish_time = start_time + path_to_start.size() + path_to_finish.size() - 2;
    return true;
}

int64_t Agent::get_finish_time() const {
    return finish_time;
}
