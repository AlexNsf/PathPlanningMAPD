#include "search.h"

void Search::DoSearch(const Map &map, Token &token, SearchResult search_result, Logger &logger) {
    for (const auto& task : map.get_tasks()) {
        token.add_task(task);
    }

    std::vector<Agent> agents;

    for (const auto& coordinate : map.get_initial_locations()) {
        agents.emplace_back(coordinate);
    }

    std::vector<Coordinate> endpoints;

    std::unordered_set<Coordinate> delivery_locations;

    for (const auto& coordinate : map.get_start_locations()) {
        endpoints.push_back(coordinate);
    }

    for (const auto& coordinate : map.get_finish_locations()) {
//        endpoints.push_back(coordinate);
        delivery_locations.insert(coordinate);
    }

    for (const auto& coordinate : map.get_initial_locations()) {
        endpoints.push_back(coordinate);
    }

    int64_t cur_ts = 0;

    auto begin = std::chrono::high_resolution_clock::now();

    while (true) {
        for (int i = 0; i < agents.size(); ++i) {
            if (agents[i].get_finish_time() <= cur_ts) {
                Task cur_task;
                bool task_set = false;
                for (const Task& task : token.get_tasks()) {
                    if ((token.get_blocked_endpoints_at_ts(cur_ts).find(task.start) == token.get_blocked_endpoints_at_ts(cur_ts).end() ||
                         token.get_blocked_endpoints_at_ts(cur_ts).find(task.start)->num == agents[i].num) &&
                        (token.get_blocked_endpoints_at_ts(cur_ts).find(task.finish) == token.get_blocked_endpoints_at_ts(cur_ts).end() ||
                         token.get_blocked_endpoints_at_ts(cur_ts).find(task.finish)->num == agents[i].num)) {
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
                if (!task_set) {
                    if (delivery_locations.find(agents[i].start_coordinate) == delivery_locations.end() &&
                        token.get_locations_at_ts(cur_ts + 1).find(agents[i].start_coordinate) == token.get_locations_at_ts(cur_ts + 1).end()) {
                        cur_task = Task(agents[i].start_coordinate, agents[i].start_coordinate);
                        agents[i].update_path(map, cur_task, token, false);
                        continue;
                    }
                    for (const Coordinate& coordinate : endpoints) {
                        if ((token.get_blocked_endpoints_at_ts(cur_ts).find(coordinate) == token.get_blocked_endpoints_at_ts(cur_ts).end() ||
                             token.get_blocked_endpoints_at_ts(cur_ts).find(coordinate)->num == agents[i].num) && agents[i].start_coordinate != coordinate) {
                            if (task_set) {
                                if (token.get_precalculated_h(agents[i].start_coordinate.i, agents[i].start_coordinate.j, coordinate.i, coordinate.j) <
                                    token.get_precalculated_h(agents[i].start_coordinate.i, agents[i].start_coordinate.j, cur_task.finish.i, cur_task.finish.j)) {
                                    cur_task = Task(agents[i].start_coordinate, coordinate);
                                }
                            } else {
                                task_set = true;
                                cur_task = Task(agents[i].start_coordinate, coordinate);
                            }
                        }
                    }
                    if (!agents[i].update_path(map, cur_task, token, false)) {
                        search_result.is_failed = true;
                        break;
                    }
                } else {
                    token.get_tasks().erase(cur_task);
                    if (!agents[i].update_path(map, cur_task, token, true)) {
                        token.add_task(cur_task);
                        task_set = false;
                        if (token.get_locations_at_ts(cur_ts + 1).find(agents[i].start_coordinate) == token.get_locations_at_ts(cur_ts + 1).end()) {
                            cur_task = Task(agents[i].start_coordinate, agents[i].start_coordinate);
                            agents[i].update_path(map, cur_task, token, false);
                            continue;
                        }
                        for (const Coordinate& coordinate : endpoints) {
                            if ((token.get_blocked_endpoints_at_ts(cur_ts).find(coordinate) == token.get_blocked_endpoints_at_ts(cur_ts).end() ||
                                 token.get_blocked_endpoints_at_ts(cur_ts).find(coordinate)->num == agents[i].num) && agents[i].start_coordinate != coordinate) {
                                if (task_set) {
                                    if (token.get_precalculated_h(agents[i].start_coordinate.i, agents[i].start_coordinate.j, coordinate.i, coordinate.j) <
                                        token.get_precalculated_h(agents[i].start_coordinate.i, agents[i].start_coordinate.j, cur_task.finish.i, cur_task.finish.j)) {
                                        cur_task = Task(agents[i].start_coordinate, coordinate);
                                    }
                                } else {
                                    task_set = true;
                                    cur_task = Task(agents[i].start_coordinate, coordinate);
                                }
                            }
                        }
                        if (!agents[i].update_path(map, cur_task, token, false)) {
                            search_result.is_failed = true;
                            break;
                        }
                    } else {
                        ++search_result.tasks_finished;
                    }
                }
            }
        }
        for (const auto& task : token.get_tasks()) {
            std::cout << task.start.i << ' ' << task.start.j << ' ' << task.finish.i << ' ' << task.finish.j << " TASK\n";
        }
        if (token.doing_task_at_ts.find(cur_ts) == token.doing_task_at_ts.end() && !token.is_any_tasks_left()) {
            break;
        }
        if (search_result.is_failed) {
            break;
        }
        logger.update_agents_locations(token.get_locations_at_ts(cur_ts));
        token.update_cur_ts();
        ++cur_ts;
    }
    logger.update_agents_locations(token.get_locations_at_ts(cur_ts));
    logger.set_end_of_logs();

    auto end = std::chrono::high_resolution_clock::now();

    search_result.makespan = cur_ts;
    search_result.throughput = search_result.tasks_finished * 100 / cur_ts;
    search_result.total_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    search_result.run_time_per_timestep = search_result.total_time / cur_ts;
    search_result.service_time = cur_ts / search_result.tasks_finished;

    std::ofstream out_file;
    out_file.open("..\\result.txt", std::ios::out | std::ios::trunc);
    out_file << "Is_failed_once " << search_result.is_failed << '\n';
    out_file << "Service_time " << search_result.service_time << '\n';
    out_file << "Throughput " << search_result.throughput << '\n';
    out_file << "Makespan " << search_result.makespan << '\n';
    out_file << "Run_time_per_timestep " << search_result.run_time_per_timestep << '\n';
    out_file << "Total_time " << search_result.total_time << '\n';
    out_file << "Tasks_finished " << search_result.tasks_finished << '\n';
}

