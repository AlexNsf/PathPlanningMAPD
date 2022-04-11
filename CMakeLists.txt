cmake_minimum_required(VERSION 3.20)
project(PathPlanningMAPD)

set(CMAKE_CXX_STANDARD 20)

add_executable(PathPlanningMAPD main.cpp searchresult.h node.h map.h map.cpp xml_consts.h token.h search.h agent.h coordinate.h task.h tinyxml2.h tinyxml2.cpp agent.cpp precalculations.cpp a_star.h a_star.cpp token.cpp precalculations.h logger.h logger.cpp)
