#include "map.h"

void find_path_len_to_all_endpoints(Map& map) {
    std::vector<Coordinate> endpoints;
    for (auto start : map.get_start_locations()) {
        endpoints.push_back(start);
    }
    for (auto finish : map.get_finish_locations()) {
        endpoints.push_back(finish);
    }
    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            if (map.CellIsTraversable(i, j)) {
                Coordinate cur_coordinate(i, j);
                for (const auto& coordinate : endpoints) {
                    if (cur_coordinate != coordinate) {
                        //A*
                    }
                }
            }
        }
    }
}
