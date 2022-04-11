#include "token.h"

Token::Token() : cur_ts(0) {};

void Token::add_blocked_cell_at_ts(Coordinate blocked_coordinate, int64_t timestep) {
    blocked_cells_per_ts[timestep].insert(blocked_coordinate);
}

bool Token::is_any_blocked_at_ts(int64_t timestep) const {
    return (blocked_cells_per_ts.find(timestep) != blocked_cells_per_ts.end());
}

const std::set<Coordinate, CoordinateComparator>& Token::get_locations_at_ts(int64_t timestep) {
    return blocked_cells_per_ts[timestep];
}

void Token::update_precalculated_hs(int i, int j, int fin_i, int fin_j, double h) {
    precalculated_hs[{fin_i, fin_j}][{i, j}] = h;
}

double Token::get_precalculated_h(int i, int j, int fin_i, int fin_j) const {
    if (precalculated_hs.find({fin_i, fin_j}) != precalculated_hs.end()) {
        if (precalculated_hs.at({fin_i, fin_j}).find({i, j}) != precalculated_hs.at({fin_i, fin_j}).end()) {
            return precalculated_hs.at({fin_i, fin_j}).at({i, j});
        }
    }
    return 0;
}

int64_t Token::get_cur_ts() const {
    return cur_ts;
}

void Token::update_cur_ts() {
    ++cur_ts;
}

void Token::add_task(const Task& task) {
    tasks.push_back(task);
}

bool Token::is_any_tasks_left() {
    return !tasks.empty();
}

Task Token::pop_task() {
    if (!this->is_any_tasks_left()) {
        throw std::runtime_error("No tasks left");
    }
    Task task = tasks.front();
    tasks.erase(tasks.begin());
    return task;
}
