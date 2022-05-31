#ifndef PATHPLANNINGMAPD_SEARCH_H
#define PATHPLANNINGMAPD_SEARCH_H

#include "searchresult.h"
#include "map.h"
#include "logger.h"
#include "token.h"
#include "chrono"
#include "agent.h"

class Search {
public:
    void DoSearch(const Map& map, Token& token, SearchResult searchResult, Logger& logger);
};

#endif //PATHPLANNINGMAPD_SEARCH_H
