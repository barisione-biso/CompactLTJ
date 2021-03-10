cmake_minimum_required(VERSION 3.10)

# set the project name
project(LTJ-CompactTries)

# include files
include_directories( ./libs/sdsl-lite/include)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Main main.cpp)

# 3rd party libs
add_subdirectory( ./libs/sdsl-lite)
target_link_libraries( Main sdsl divsufsort divsufsort64 )