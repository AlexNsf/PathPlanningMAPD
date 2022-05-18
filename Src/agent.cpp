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

bool Agent::update_path(const Map& map, const Task& task, Token& token, bool real_task) {
//    std::cout << start_coordinate.i << ' ' << start_coordinate.j << " START\n";
    start_coordinate.num = this->num;
    start_time = token.get_cur_ts();
    path_to_start.clear();
    if (task.start == task.finish) {
//        std::cout << start_coordinate.i << ' ' << start_coordinate.j << " FINISH\n";
        start_coordinate.prev_i = start_coordinate.i;
        start_coordinate.prev_j = start_coordinate.j;
        token.add_blocked_cell_at_ts(start_coordinate, start_time + 1);
        finish_time = start_time + 1;
        return true;
    }
    AStar search_to_start;
    Task task_to_start(start_coordinate, task.start);
//    std::cout << task_to_start.start.i << ' ' << task_to_start.start.j << ' ' << task_to_start.finish.i << ' ' << task_to_start.finish.j << "\n";
    auto search_res_to_start = search_to_start.find_path(map, task_to_start, token, start_time);
    if (!search_res_to_start.is_found) {
        start_coordinate.prev_i = start_coordinate.i;
        start_coordinate.prev_j = start_coordinate.j;
        token.add_blocked_cell_at_ts(start_coordinate, start_time);
        token.add_blocked_cell_at_ts(start_coordinate, start_time + 1);
        token.add_blocked_endpoint(start_coordinate, start_time);
        token.add_blocked_endpoint(start_coordinate, start_time + 1);
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
        token.add_blocked_cell_at_ts(start_coordinate, start_time);
        token.add_blocked_cell_at_ts(start_coordinate, start_time + 1);
        token.add_blocked_endpoint(start_coordinate, start_time);
        token.add_blocked_endpoint(start_coordinate, start_time + 1);
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
        if (token.get_locations_at_ts(start_time + i).find(path_to_start[i]) !=
        token.get_locations_at_ts(start_time + i).end()) {
        }
        token.add_blocked_cell_at_ts(path_to_start[i], start_time + i);
//        start_coordinate.num = this->num;
//        token.add_blocked_endpoint(start_coordinate, start_time + i); // занятый финиш
        Coordinate fin = task.finish;
        fin.num = this->num;
        token.add_blocked_endpoint(fin, start_time + i);
        if (real_task) {
            token.doing_task_at_ts.insert(start_time + i);
        }
    }
    for (int i = 0; i < path_to_finish.size(); ++i) {
        if (i == 0) {
            path_to_finish[i].prev_i = path_to_start[path_to_start.size() - 2].i;
            path_to_finish[i].prev_j = path_to_start[path_to_start.size() - 2].j;
            if (real_task) {
                path_to_finish[i].started = true;
            }
        } else {
            path_to_finish[i].prev_i = path_to_finish[i - 1].i;
            path_to_finish[i].prev_j = path_to_finish[i - 1].j;
        }
        if (i == path_to_finish.size() - 1) {
            if (real_task) {
                path_to_finish[i].finished = true;
            }
        }
        path_to_finish[i].num = this->num;
        if (token.get_locations_at_ts(start_time + search_res_to_start.path_len + i).find(path_to_finish[i]) !=
        token.get_locations_at_ts(start_time + search_res_to_start.path_len + i).end()) {
        }
        token.add_blocked_cell_at_ts(path_to_finish[i], start_time + search_res_to_start.path_len + i);
//        start_coordinate.num = this->num;
//        token.add_blocked_endpoint(start_coordinate, start_time + search_res_to_start.path_len + i);
        Coordinate fin = task.finish;
        fin.num = this->num;
        token.add_blocked_endpoint(fin, start_time + search_res_to_start.path_len + i); // занятый финиш
        if (real_task) {
            token.doing_task_at_ts.insert(start_time + search_res_to_start.path_len + i);
        }
    }
    start_coordinate = task.finish;
    finish_time = start_time + path_to_start.size() + path_to_finish.size() - 2;
    return true;
}

int64_t Agent::get_finish_time() const {
    return finish_time;
}
