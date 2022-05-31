cmake_minimum_required(VERSION 3.20)
project(PathPlanningMAPD)

set(CMAKE_CXX_STANDARD 20)

add_executable(PathPlanningMAPD main.cpp Src/searchresult.h Src/node.h Src/map.h Src/map.cpp Src/xml_consts.h Src/token.h Src/search.h Src/agent.h Src/coordinate.h Src/task.h Src/tinyxml2.h Src/tinyxml2.cpp Src/agent.cpp Src/precalculations.cpp Src/a_star.h Src/a_star.cpp Src/token.cpp Src/precalculations.h Src/logger.h Src/logger.cpp Src/search.cpp)
