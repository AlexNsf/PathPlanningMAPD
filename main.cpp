#include <iostream>
#include "map.h"
#include "searchresult.h"
#include "precalculations.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    Map map;
    map.get_map(argv[1]);
    Token token;
    find_path_len_to_all_endpoints(map, token);
    SearchResult search_result;
}
