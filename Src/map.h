#ifndef PATHPLANNINGMAPD_MAP_H
#define PATHPLANNINGMAPD_MAP_H

#include "coordinate.h"
#include "tinyxml2.h"
#include "xml_consts.h"
#include "task.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

class Map {
private:
    int height, width;
    std::vector<Coordinate> initial_locations;
    std::vector<Coordinate> start_locations;
    std::vector<Coordinate> finish_locations;
    std::vector<Task> tasks;
    int** Grid;

public:
    Map();

    ~Map();

    bool get_map(const char *FileName);

    bool CellIsTraversable (int i, int j) const;

    bool CellOnGrid (int i, int j) const;

    bool CellIsObstacle(int i, int j) const;

    int  getValue(int i, int j) const;

    int getMapHeight() const;

    int getMapWidth() const;

    const std::vector<Coordinate>& get_initial_locations() const;

    const std::vector<Coordinate>& get_start_locations() const;

    const std::vector<Coordinate>& get_finish_locations() const;

    const std::vector<Task>& get_tasks() const;
};

#endif //PATHPLANNINGMAPD_MAP_H
