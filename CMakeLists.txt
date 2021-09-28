cmake_minimum_required(VERSION 3.10)

# set the project name
project(LTJ-CompactTries)

# include files
include_directories( 
    # ~/libs/sdsl-lite/external/libdivsufsort/include/
    # ~/libs/sdsl-lite/include
    # ./libs/sdsl-lite/lib 
    ./include
    )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
        # ${CMAKE_HOME_DIRECTORY}/include
        ${CMAKE_BINARY_DIR}/libs/sdsl-lite/include
        ${CMAKE_BINARY_DIR}/libs/sdsl-lite/external/libdivsufsort/include/
        # ${CMAKE_HOME_DIRECTORY}/external/googletest/include
        # ${CMAKE_HOME_DIRECTORY}/external/sdsl-lite/external/FastPFor/headers/
        # ${CMAKE_HOME_DIRECTORY}/external/repair_gn
        # ${STXXL_INCLUDE_DIRS}
        )

# link_directories(
#     ./libs/sdsl-lite/external/libdivsufsort/include/
#     ./libs/sdsl-lite/lib)

# link_libraries(sdsl divsufsort divsufsort64)

# add the executable
add_executable(main src/main.cpp)
add_executable(build_index src/build_index.cpp)

# 3rd party libs
add_subdirectory(./libs/sdsl-lite)
target_link_libraries( main sdsl divsufsort divsufsort64)
target_link_libraries( build_index sdsl divsufsort divsufsort64)