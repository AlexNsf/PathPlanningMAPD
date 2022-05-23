#include "precalculations.h"

void find_path_len_to_all_endpoints(const Map& map, Token& token) {
    std::vector<Coordinate> endpoints;
    for (auto start : map.get_start_locations()) {
        endpoints.push_back(start);
    }
    for (auto finish : map.get_finish_locations()) {
        endpoints.push_back(finish);
    }
    for (auto initial : map.get_initial_locations()) {
        endpoints.push_back(initial);
    }
//    std::vector<std::vector<double>> precalculated_hs(map.getMapHeight(), std::vector<double>(map.getMapWidth(), 0));
    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            if (map.CellIsTraversable(i, j)) {
                Coordinate cur_coordinate(i, j);
                for (const auto& end_coordinate : endpoints) {
                    if (cur_coordinate != end_coordinate) {
                        Task cur_task(cur_coordinate, end_coordinate);
                        AStar cur_search;
                        auto search_res = cur_search.find_path(map, cur_task, token, 0);
                        if (search_res.is_found) {
                            token.update_precalculated_hs(i, j, end_coordinate.i, end_coordinate.j, search_res.path_len);
                        }
                    }
                }
            }
        }
    }
}
