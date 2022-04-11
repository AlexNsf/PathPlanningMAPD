#ifndef PATHPLANNINGMAPD_TOKEN_H
#define PATHPLANNINGMAPD_TOKEN_H

#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include "task.h"

class Token {
private:
    int64_t cur_ts;
    std::unordered_map<int64_t, std::set<Coordinate, CoordinateComparator>> blocked_cells_per_ts; // for each timestep set of blocked coordinates (where agents locate)
    std::list<Task> tasks;
    // нужно поменять set на unordered_set
    std::map<std::pair<int, int>, std::map<std::pair<int, int>, double>> precalculated_hs;
    // неплохо бы поменять на unordered_map оба map

public:
    Token();
//    std::unordered_map<int64_t, std::set<Coordinate, CoordinateComparator>> blocked_cells_per_ts;
//    std::map<std::pair<int, int>, std::map<std::pair<int, int>, double>> precalculated_hs;

    void add_blocked_cell_at_ts(Coordinate blocked_coordinate, int64_t timestep);

    bool is_any_blocked_at_ts(int64_t timestep) const;

    const std::set<Coordinate, CoordinateComparator>& get_locations_at_ts(int64_t timestep);

    void update_precalculated_hs(int i, int j, int fin_i, int fin_j, double h);

    double get_precalculated_h(int i, int j, int fin_i, int fin_j) const;

    int64_t get_cur_ts() const;

    void update_cur_ts();

    void add_task(const Task&);

    bool is_any_tasks_left();

    Task pop_task();
};

#endif //PATHPLANNINGMAPD_TOKEN_H