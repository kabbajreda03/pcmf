cmake_minimum_required(VERSION 3.14)

project(OptionPricer CXX)
find_package(nlohmann_json REQUIRED)
set(SOURCE_FILES MonteCarloPricer.cpp MonteCarloRoutine.cpp)
set(HEADERS MonteCarloPricer.hpp MonteCarloRoutine.hpp)

add_library(optionPricer ${SOURCE_FILES} ${HEADERS})

