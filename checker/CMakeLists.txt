cmake_minimum_required(VERSION 3.10)
project(jhelper-example-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# use lld to speed up linking


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")


add_executable(my my.cpp)
add_executable(ac ac.cpp)
target_compile_definitions(my PRIVATE LOCAL)

