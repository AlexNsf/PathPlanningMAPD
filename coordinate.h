#ifndef PATHPLANNINGMAPD_COORDINATE_H
#define PATHPLANNINGMAPD_COORDINATE_H

#include <tuple>

struct Coordinate {
    int i, j;

    Coordinate() : i(-1), j(-1) {}

    Coordinate(int i_, int j_) : i(i_), j(j_) {}
};

inline bool operator==(const Coordinate& c_1, const Coordinate& c_2) {
    return c_1.i == c_2.i && c_1.j == c_2.j;
}

inline bool operator!=(const Coordinate& c_1, const Coordinate& c_2) {
    return !(c_1 == c_2);
}

struct CoordinateComparator {
    bool operator()(const Coordinate& c_1, const Coordinate& c_2) const {
        return std::tie(c_1.i, c_1.j) < std::tie(c_2.i, c_2.j);
    }
};

#endif //PATHPLANNINGMAPD_COORDINATE_H
