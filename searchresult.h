#ifndef PATHPLANNINGMAPD_SEARCHRESULT_H
#define PATHPLANNINGMAPD_SEARCHRESULT_H

#include <cstdint>

struct SearchResult {
    bool is_failed;
    int64_t service_time; // average number of timesteps needed to finish executing each task after it enters the system
    int64_t throughput; // tasks executed per 100 timesteps
    int64_t makespan; // timestep when all tasks are finished
    double run_time_per_timestep;
    double total_time;
    int64_t tasks_finished;

    SearchResult() : is_failed(false), service_time(0), throughput(0), makespan(0), run_time_per_timestep(0),
                     total_time(0), tasks_finished(0) {}
};

#endif //PATHPLANNINGMAPD_SEARCHRESULT_H
