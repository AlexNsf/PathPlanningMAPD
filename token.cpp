#include "token.h"

Token::Token() : cur_ts(0) {};

void Token::add_blocked_cell_at_ts(Coordinate blocked_coordinate, int64_t timestep) {
    blocked_cells_per_ts[timestep].insert(blocked_coordinate);
}

bool Token::is_any_blocked_at_ts(int64_t timestep) const {
    return (blocked_cells_per_ts.find(timestep) != blocked_cells_per_ts.end());
}

const std::set<Coordinate, CoordinateComparator> &Token::get_locations_at_ts(int64_t timestep) const {
    return blocked_cells_per_ts.at(timestep);
}

void Token::update_precalculated_hs(int i, int j, int fin_i, int fin_j, double h) {
    precalculated_hs[{fin_i, fin_j}][i][j] = h;
}

double Token::get_precalculated_h(int i, int j, int fin_i, int fin_j) const {
    return precalculated_hs.at({fin_i, fin_j})[i][j];
}

int64_t Token::get_cur_ts() const {
    return cur_ts;
}

void Token::update_cur_ts() {
    ++cur_ts;
}
