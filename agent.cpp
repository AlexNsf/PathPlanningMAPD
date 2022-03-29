#include "agent.h"

Agent::Agent() {
    start_time = -1;
    is_busy = false;
}

Agent::Agent(Coordinate initial_position) {
    end_point = initial_position;
    is_busy = false;
}

void Agent::update_path(Task task) {
    end_point = task.finish;
    is_busy = true;
    // A*
}
