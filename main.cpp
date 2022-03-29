#include <iostream>
#include "map.h"
#include "searchresult.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    Map map;
    map.get_map(argv[1]);
    SearchResult search_result;
}
