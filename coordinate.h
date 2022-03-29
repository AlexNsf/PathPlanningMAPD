#ifndef PATHPLANNINGMAPD_COORDINATE_H
#define PATHPLANNINGMAPD_COORDINATE_H

struct Coordinate {
    int i, j;

    Coordinate() : i(-1), j(-1) {}

    Coordinate(int i_, int j_) : i(i_), j(j_) {}
};

bool operator==(Coordinate c_1, Coordinate c_2) {
    return c_1.i == c_2.i && c_1.j == c_2.j;
}

bool operator!=(Coordinate c_1, Coordinate c_2) {
    return !(c_1 == c_2);
}

#endif //PATHPLANNINGMAPD_COORDINATE_H
