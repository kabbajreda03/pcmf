cmake_minimum_required(VERSION 3.14)

project(Options CXX)

set(SOURCE_FILES ConditionalBasketOption.cpp ConditionalMaxOption.cpp)
set(HEADERS ConditionalBasketOption.hpp ConditionalMaxOption.hpp Option.hpp OptionParameters.hpp)

add_library(options ${SOURCE_FILES} ${HEADERS})