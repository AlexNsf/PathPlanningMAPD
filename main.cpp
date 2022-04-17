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
        logger.set_out_file("..\\logs\\log.txt");
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
    }

    int64_t cur_ts = 0;

    while (cur_ts <= 200) {
//        std::cout << token.is_any_tasks_left() << '\n';
        while (token.is_any_tasks_left()) {
            bool flag = false;
            for (int i = 0; i < agents.size(); ++i) {
                if (agents[i].get_finish_time() <= cur_ts) {
                    Task cur_task;
                    bool task_set = false;
                    for (const Task& task : token.get_tasks()) {
                        if (token.get_blocked_endpoints_at_ts(cur_ts).find(task.start) == token.get_blocked_endpoints_at_ts(cur_ts).end() &&
                                 token.get_blocked_endpoints_at_ts(cur_ts).find(task.finish) == token.get_blocked_endpoints_at_ts(cur_ts).end()) {
                            if (task_set) {
                                if (token.get_precalculated_h(agents[i].start_coordinate.i, agents[i].start_coordinate.j, task.start.i, task.start.j) <
                                         token.get_precalculated_h(agents[i].start_coordinate.i, agents[i].start_coordinate.j, cur_task.start.i, cur_task.start.j)) {
                                    cur_task = task;
                                }
                            } else {
                                task_set = true;
                                cur_task = task;
                            }
                        }
                    }
//                    Task cur_task = *token.get_tasks().begin();
                    if (!task_set) {
                        flag = true;
                        break;
                    }
                    token.get_tasks().erase(cur_task);
                    if (!agents[i].update_path(map, cur_task, token)) {
                        token.add_task(cur_task);
                    }
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


//    for (const auto& c : token.blocked_endpoints_per_ts) {
//        std::cout << c.first << " NEW COORD\n";
//        for (const auto& coord : c.second) {
//            std::cout << coord.i << ' ' << coord.j << '\n';
//        }
//    }

//    for (const auto& agent : agents) {
//        std::cout << agent.num << '\n';
//    }
}
