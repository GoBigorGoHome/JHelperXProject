cmake_minimum_required(VERSION 3.10)
project(jhelper-example-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(testrunner testrunner/main.cpp testrunner/tester.cpp)
# use libstdc++ debug mode
target_compile_definitions(testrunner PRIVATE _GLIBCXX_DEBUG)
# use lld to speed up linking
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(testrunner PRIVATE -Wno-shift-op-parentheses)
    if (NOT CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        # fix linking error when using std::future with Clang
        # see https://stackoverflow.com/a/31639828/6793559 for detail
        target_compile_options(testrunner PRIVATE -femulated-tls)
    endif ()
endif ()

if (MSVC)
    #    target_compile_options(testrunner PRIVATE -fsanitize=address /MT)
    #    target_link_options(testrunner PRIVATE /libpath:D:\\LLVM\\lib\\clang\\10.0.0\\lib\\windows)
    #    target_link_libraries(testrunner PRIVATE clang_rt.asan-x86_64.lib)
endif ()
add_library(mylib INTERFACE)
target_compile_definitions(mylib INTERFACE LOCAL)
target_include_directories(mylib INTERFACE library)
target_link_libraries(testrunner PRIVATE mylib)

# to enable code insight for task.cpp, task.cpp must be source of some target.
# JHelper will refresh CMake when new tasks are added.
file(GLOB TASKS "tasks/*.cpp")
add_library(tasks ${TASKS})
target_link_libraries(tasks PRIVATE mylib)

