#include <iostream>
#include "Src/map.h"
#include "Src/searchresult.h"
#include "Src/search.h"
#include "Src/precalculations.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    Map map;
    map.get_map(argv[1]);

    Logger logger;
    if (argc < 3) {
        logger.set_out_file("..\\logs\\log.txt");
    } else {
        logger.set_out_file(argv[2]);
    }
    logger.save_map(map);

    Token token;
    find_path_len_to_all_endpoints(map, token);

    SearchResult search_result;

    Search search;
    search.DoSearch(map, token, search_result, logger);
}
