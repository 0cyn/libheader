cmake_minimum_required(VERSION 3.21)
project(libHeader)

set(CMAKE_CXX_STANDARD 17)

# add SHARED after "Header" if you need this as a dylib. It will be static (.a) by default.
add_library(Header Header.cpp Header.h)

# add_executable(headers headerTest.cpp)
# target_link_libraries(headers Header)
