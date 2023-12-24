cmake_minimum_required(VERSION 3.13)
project(jhelper-example-project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(solution
        testrunner/current_task.cpp
        solution/main.cpp)

add_executable(testrunner
        testrunner/current_task.cpp
        testrunner/main.cpp
        testrunner/test_runner.cpp
        testrunner/compare_floats.cpp
        testrunner/string_utils.cpp
        testrunner/checker.cpp
        testrunner/print_test.cpp
        testrunner/color.cpp)

add_dependencies(testrunner solution)
target_compile_definitions(testrunner PRIVATE LOCAL)
# 不能没有引号
target_compile_definitions(testrunner PRIVATE SOLUTION_EXE="${CMAKE_BINARY_DIR}/solution")
target_include_directories(testrunner PRIVATE library)

# 一般情况不要使用ASAN。用ASAN会让程序变慢。本来不会超时的题目用了ASAN可能回超时，例如https://atcoder.jp/contests/abc319/submissions/45391784
#if (NOT WIN32 OR DEFINED ENV{ASAN})
#    target_compile_options(solution PRIVATE -O1 -fsanitize=address -fno-omit-frame-pointer)
#    target_link_options(solution PRIVATE -fsanitize=address)
#endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # specify standards conformance mode
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /Zc:preprocessor /Zc:__cplusplus")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=vla")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
    # Use mold on Linux or lld on Windows
    if (EXISTS "/usr/bin/mold")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
    else ()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    endif ()
    # Set stack size to 256 MiB
    if (WIN32)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,268435456")
    else ()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,stack-size=268435456")
    endif ()
    #    target_compile_options(testrunner PRIVATE -Werror)
    target_compile_options(testrunner PRIVATE -Werror=conversion)
    target_compile_options(testrunner PRIVATE -Wno-sign-conversion)
    target_compile_options(testrunner PRIVATE -Wno-float-conversion)
    target_compile_options(testrunner PRIVATE -Wno-trigraphs)
    target_compile_options(testrunner PRIVATE -Wno-sign-compare)
    target_compile_options(testrunner PRIVATE -Wno-parentheses)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(testrunner PRIVATE -Wno-maybe-uninitialized)
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(testrunner PRIVATE -Wno-bool-compare)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(testrunner PRIVATE -Wno-shift-op-parentheses)
    target_compile_options(testrunner PRIVATE -Wno-logical-op-parentheses)
    target_compile_options(testrunner PRIVATE -Wno-missing-braces)
    target_compile_options(testrunner PRIVATE -Wno-implicit-int-float-conversion)
    target_compile_options(testrunner PRIVATE -Wno-float-conversion)
    if (WIN32 AND NOT CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        # fix linking error when using std::future with Clang
        # see https://stackoverflow.com/a/31639828/6793559 for detail
        target_compile_options(testrunner PRIVATE -femulated-tls)
    endif ()
endif ()

include_directories(library)

add_executable(exp exp.cpp)
target_compile_definitions(exp PRIVATE LOCAL)

add_executable(chk chk.cpp)

add_executable(my checker/my.cpp)
target_compile_definitions(my PRIVATE LOCAL)
add_executable(ac checker/ac.cpp)
include_directories(ac-library)
include_directories(tourist-library)

#if (NOT WIN32 OR DEFINED ENV{ASAN})
#    target_compile_options(exp PRIVATE -O1 -fsanitize=address -fno-omit-frame-pointer)
#    target_link_options(exp PRIVATE -fsanitize=address)
#endif ()

# 打印CMake的变量
#get_cmake_property(_variableNames VARIABLES)
#list(SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach ()