cmake_minimum_required(VERSION 3.10)
project(jhelper-example-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(testrunner
        testrunner/main.cpp
        testrunner/test_runner.cpp
        testrunner/compare_floats.cpp
        testrunner/string_utils.cpp
        testrunner/checker.cpp
        testrunner/print_test.cpp
        testrunner/color.cpp)
target_include_directories(testrunner PRIVATE library)
# use libstdc++ debug mode
target_compile_definitions(testrunner PRIVATE _GLIBCXX_DEBUG)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # specify standards conformance mode
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /Zc:preprocessor /Zc:__cplusplus")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=conversion")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=vla")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-conversion")
    # use lld to speed up linking
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    # Set stack size to 256 MiB
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,268435456")
endif ()

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

include_directories(library)

add_executable(exp exp.cpp)

add_executable(my checker/my.cpp)
target_compile_definitions(my PRIVATE LOCAL)
add_executable(ac checker/ac.cpp)
