cmake_minimum_required(VERSION 3.14)

project(Models CXX)

set(SOURCE_FILES BlackScholesModel.cpp BlackScholesHelper.cpp)
set(HEADERS UnderlyingModel.hpp BlackScholesModel.hpp BlackScholesHelper.hpp BlackScholesModelParameters.hpp)

add_library(models STATIC ${SOURCE_FILES} ${HEADERS})
target_link_libraries(models ${LIBS})