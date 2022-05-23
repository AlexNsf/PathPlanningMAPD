#ifndef PATHPLANNINGMAPD_TOKEN_H
#define PATHPLANNINGMAPD_TOKEN_H

#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "task.h"

namespace std {
    template<>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const pair<int, int> &k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            return (hash<int>()(k.first)
                    ^ (hash<int>()(k.second)));
        }
    };
}

class Token {
private:
    int64_t cur_ts;
    std::unordered_map<int64_t, std::unordered_set<Coordinate>> blocked_cells_per_ts; // for each timestep set of blocked coordinates (where agents locate)
    std::unordered_set<Task> tasks;
    // нужно поменять set на unordered_set
//    std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, double>> precalculated_hs;
    // неплохо бы поменять на unordered_map оба map
    std::unordered_map<int64_t, std::unordered_set<Coordinate>> blocked_endpoints_per_ts;

public:
    Token();
//    std::unordered_map<int64_t, std::set<Coordinate, CoordinateComparator>> blocked_endpoints_per_ts;
//    std::unordered_map<int64_t, std::set<Coordinate, CoordinateComparator>> blocked_cells_per_ts;
    std::map<std::pair<int, int>, std::map<std::pair<int, int>, double>> precalculated_hs;

    std::unordered_set<int64_t> doing_task_at_ts;

    void add_blocked_cell_at_ts(Coordinate blocked_coordinate, int64_t timestep);

    bool is_any_blocked_at_ts(int64_t timestep) const;

    const std::unordered_set<Coordinate>& get_locations_at_ts(int64_t timestep);

    void update_precalculated_hs(int i, int j, int fin_i, int fin_j, double h);

    double get_precalculated_h(int i, int j, int fin_i, int fin_j) const;

    int64_t get_cur_ts() const;

    void update_cur_ts();

    void add_task(const Task&);

    bool is_any_tasks_left();

//    Task pop_task();

    void add_blocked_endpoint(Coordinate blocked_endpoint, int64_t timestep);

    bool is_any_blocked_endpoints_at_ts(int64_t timestep) const;

    const std::unordered_set<Coordinate>& get_blocked_endpoints_at_ts(int64_t timestep);

    std::unordered_set<Task>& get_tasks();
};

#endif //PATHPLANNINGMAPD_TOKEN_H
