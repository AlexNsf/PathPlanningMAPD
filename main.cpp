#include <iostream>
#include "map.h"
#include "searchresult.h"
#include "precalculations.h"
#include "logger.h"
#include "agent.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    Map map;
    map.get_map(argv[1]);

    Logger logger;
    if (argc < 3) {
        logger.set_out_file("..\\log.txt");
    } else {
        logger.set_out_file(argv[2]);
    }
    logger.save_map(map);

    Token token;
    find_path_len_to_all_endpoints(map, token);

//    for (int i = 0; i < map.getMapHeight(); ++i) {
//        for (int j = 0; j < map.getMapWidth(); ++j) {
//            std::cout << token.precalculated_hs[{0, 0}][{i, j}] << ' ';
//        }
//        std::cout << '\n';
//    }

    SearchResult search_result;


    for (const auto& task : map.get_tasks()) {
        token.add_task(task);
    }

    std::vector<Agent> agents;

    for (const auto& coordinate : map.get_initial_locations()) {
        agents.emplace_back(coordinate);
        token.add_blocked_cell_at_ts(coordinate, 0);
    }

    int64_t cur_ts = 0;

    while (cur_ts <= 100) {
//        std::cout << token.is_any_tasks_left() << '\n';
        while (token.is_any_tasks_left()) {
            bool flag = false;
            for (int i = 0; i < agents.size(); ++i) {
                if (agents[i].get_finish_time() <= cur_ts) {
                    agents[i].update_path(map, token.pop_task(), token);
                    break;
                } else if (i == agents.size() - 1) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                break;
            }
        }
        logger.update_agents_locations(token.get_locations_at_ts(cur_ts));
        token.update_cur_ts();
        ++cur_ts;
    }
    logger.update_agents_locations(token.get_locations_at_ts(cur_ts));
    logger.set_end_of_logs();
//    for (const auto& c : token.blocked_cells_per_ts) {
//        std::cout << c.first << " NEW COORD\n";
//        for (const auto& coord : c.second) {
//            std::cout << coord.i << ' ' << coord.j << '\n';
//        }
//    }
}
