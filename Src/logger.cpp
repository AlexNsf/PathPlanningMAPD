#include "logger.h"


Logger::Logger() = default;

Logger::Logger(const char *FileName) {
    out_file.open(FileName, std::ios::out | std::ios::trunc);
}

Logger::~Logger() {
    out_file.close();
}

void Logger::set_out_file(const char *FileName) {
    out_file.open(FileName, std::ios::out | std::ios::trunc);
}

void Logger::set_size(const Map& map) {
    out_file << "Size:\n" << map.getMapHeight() << ' ' << map.getMapWidth() << '\n';
}

void Logger::set_obstacles(const Map& map) {
    out_file << "Obstacles:\n";
    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            if (map.CellIsObstacle(i, j) == 1) {
                out_file << j << ' ' << i << '\n';
            }
        }
    }
}

void Logger::set_initials(const std::vector<Coordinate> &initials) {
    out_file << "Initial positions:\n";
    for (int i = 0; i < initials.size(); ++i) {
        out_file << initials[i].j << ' ' << initials[i].i << ' ' << i + 1 << " 0 0\n";
    }
}

void Logger::set_starts(const std::vector<Coordinate> &starts) {
    out_file << "Start positions:\n";
    for (const auto &start: starts) {
        out_file << start.j << ' ' << start.i << '\n';
    }
}

void Logger::set_finishes(const std::vector<Coordinate> &finishes) {
    out_file << "Finish positions:\n";
    for (const auto &finish: finishes) {
        out_file << finish.j << ' ' << finish.i << '\n';
    }
}

void Logger::update_agents_locations(const std::unordered_set<Coordinate>& new_locations) {
    out_file << "New move:\n";
    for (const auto& new_location : new_locations) {
        out_file << new_location.j << ' ' << new_location.i << ' ' << new_location.num << ' ' <<
                 new_location.started << ' ' << new_location.finished << '\n';
    }
}

void Logger::save_map(const Map &map) {
    this->set_size(map);
    this->set_obstacles(map);
    this->set_initials(map.get_initial_locations());
    this->set_starts(map.get_start_locations());
    this->set_finishes(map.get_finish_locations());
}

void Logger::set_end_of_logs() {
    out_file << "END OF LOGS\n";
}

