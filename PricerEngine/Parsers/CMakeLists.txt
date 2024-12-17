cmake_minimum_required(VERSION 3.14)

project(Parsers CXX)
find_package(nlohmann_json REQUIRED)
set(SOURCE_FILES CoreBlackScholesModelInputParser.cpp CoreOptionInputParser.cpp json_helper.cpp)
set(HEADERS BlackScholesModelInputParser.hpp BlackScholesModelInputParser.hpp CoreOptionInputParser.hpp json_helper.hpp OptionInputParser.hpp)

add_library(parsers ${SOURCE_FILES} ${HEADERS})
target_link_libraries(parsers
	nlohmann_json::nlohmann_json
)