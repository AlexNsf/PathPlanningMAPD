#ifndef PATHPLANNINGMAPD_LOGGER_H
#define PATHPLANNINGMAPD_LOGGER_H

#include <fstream>
#include <vector>
#include <set>
#include "coordinate.h"
#include "map.h"

class Logger {
private:
    std::ofstream out_file;

public:
    Logger();

    Logger(const char* FileName);

    ~Logger();

    void set_out_file(const char* FileName);

    void set_size(const Map& map);

    void set_obstacles(const Map& map);

    void set_initials(const std::vector<Coordinate>& initials);

    void set_starts(const std::vector<Coordinate>& starts);

    void set_finishes(const std::vector<Coordinate>& starts);

    void update_agents_locations(const std::set<Coordinate, CoordinateComparator>& new_locations);

    void save_map(const Map& map);

    void set_end_of_logs();
};

#endif //PATHPLANNINGMAPD_LOGGER_H
